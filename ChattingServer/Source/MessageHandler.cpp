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
	case Message::EPayloadType::Chatting:
		std::cout << "WOW!!\n";
		std::cout << message->Payload.chatting.Message << std::endl;
		break;
	case Message::EPayloadType::Friend:
		std::cout << "LOL!!\n";
		std::cout << "from: " << message->Payload.friendmsg.Sender << " target: "
			<< message->Payload.friendmsg.Target << std::endl;
	case Message::EPayloadType::System:
		if (message->Payload.system.Type == Message::ESystemMessageType::Login)
		{
			std::cout << "Great!!\n";
			std::cout << "Name: " << message->Payload.system.Payload << std::endl;
		}
	}

	Server->ReleaseMessage(message);
}