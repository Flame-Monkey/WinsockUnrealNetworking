#include "MessageHandler.h"
#include <iostream>
#include <mutex>

MessageHandler::MessageHandler(ChattingServer* server, int maxThread):
	Server(server), HandlerLock(), HandlerCV(), MessageQueue(), ThreadPool(nullptr), MaxThread(maxThread)
{
}
void MessageHandler::Init()
{
	ThreadPool = new std::thread[MaxThread];
}

void MessageHandler::Start()
{
	for (int i = 0; i < MaxThread; ++i)
	{
		std::cout << "Create new WorkerThread(Handler) #" << i << std::endl;
		ThreadPool[i] = std::thread(WorkerThread, this, i);
	}
}

void MessageHandler::WorkerThread(MessageHandler* handler, int i)
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(handler->HandlerLock);
		handler->HandlerCV.wait(lock, [handler] {
			return !handler->MessageQueue.empty();
			});
		auto [manager, message] = handler->MessageQueue.front();
		handler->MessageQueue.pop();
		lock.unlock();

		handler->HandleMessage(manager, message);
	}
}

void MessageHandler::PushMessage(SocketManager* manager, Message::StructMessage* message)
{
	std::unique_lock<std::mutex> lock(HandlerLock);
	MessageQueue.emplace(manager, message);
	HandlerCV.notify_one();
}

void MessageHandler::HandleMessage(SocketManager*& manager, Message::StructMessage* message)
{
	switch (message->Type)
	{
	case Message::EPayloadType::System:
		HandleSystemMessage(manager, message);
		break;
	case Message::EPayloadType::Chatting:
		HandleChattingMessage(manager, message);
		break;
	case Message::EPayloadType::Friend:
		HandleFriendMessage(manager, message);
		break;
	}

	Server->ReleaseMessage(message);
}

void MessageHandler::HandleSystemMessage(SocketManager*& manager, Message::StructMessage* message)
{
	if (message->Payload.system.Type == Message::ESystemMessageType::Login)
	{
		std::cout << "Great!!\n";
		std::string name(message->Payload.system.Payload);
		std::cout << "Name: " << name << " name length: " << name.length() << std::endl;
		if (ConnectedClients.find(name) == ConnectedClients.end())
		{
			ConnectedClients.insert({ name, manager });

			Message::MessagePayload p;
			p.system = Message::SystemMessage(Message::ESystemMessageType::Login, "Success");
			Message::StructMessage m(p, Message::EPayloadType::System);

			manager->PushMessageSendQueue(m);
		}
		else
		{
			Message::MessagePayload p;
			p.system = Message::SystemMessage(Message::ESystemMessageType::Login, "Fail");
			Message::StructMessage m(p, Message::EPayloadType::System);

			manager->PushMessageSendQueue(m);
		}
	}
}

void MessageHandler::HandleChattingMessage(SocketManager*& manager, Message::StructMessage* message)
{
	std::cout << "WOW!!\n";
	std::cout << message->Payload.chatting.Message << std::endl;
}

void MessageHandler::HandleFriendMessage(SocketManager*& manager, Message::StructMessage* message)
{
	std::cout << "LOL!!\n";
	std::cout << "from: " << message->Payload.friendmsg.Sender << " target: "
		<< message->Payload.friendmsg.Target << std::endl;
}