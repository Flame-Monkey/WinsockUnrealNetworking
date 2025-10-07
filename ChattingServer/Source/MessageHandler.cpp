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
	HeartBeatThread = new std::thread(HeartBeatSeeker, this);
}

void MessageHandler::WorkerThread(MessageHandler* handler, int i)
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(handler->HandlerLock);
		handler->HandlerCV.wait(lock, [handler] {
			return !handler->MessageQueue.empty();
			});
		auto [manager, message, socketSessionNumber] = handler->MessageQueue.front();
		handler->MessageQueue.pop();
		lock.unlock();

		handler->HandleMessage(manager, socketSessionNumber, message);
	}
}

void MessageHandler::PushMessage(SocketManager* manager, Message::StructMessage* message, int sessionNumber)
{
	std::unique_lock<std::mutex> lock(HandlerLock);
	MessageQueue.emplace(manager, message, sessionNumber);
	HandlerCV.notify_one();
}

void MessageHandler::HandleMessage(SocketManager*& manager, int sessionNumber, Message::StructMessage* message)
{
	switch (message->Type)
	{
	case Message::EPayloadType::System:
		HandleSystemMessage(manager, sessionNumber, message);
		break;
	case Message::EPayloadType::Chatting:
		HandleChattingMessage(manager, sessionNumber, message);
		break;
	case Message::EPayloadType::Friend:
		HandleFriendMessage(manager, sessionNumber, message);
		break;
	}

	Server->ReleaseMessage(message);
}

void MessageHandler::HandleSystemMessage(SocketManager*& manager, int sessionNumber, Message::StructMessage* message)
{
	std::string name;
	switch(message->Payload.system.Type)
	{
	case Message::ESystemMessageType::Login:
		std::cout << "Great!!\n";
		name = (message->Payload.system.Payload);
		std::cout << "Name: " << name << " name length: " << name.length() << std::endl;
		if (LoginedClients.find(name) == LoginedClients.end())
		{
			manager->LastResponse = message->Header.TimeStamp;
			LoginLock.lock();
			LoginedClients.insert({ name, {manager, sessionNumber } });
			LoginLock.unlock();
			manager->SetClientName(name);

			Message::MessagePayload p;
			p.system = Message::SystemMessage(Message::ESystemMessageType::Login, "Success");
			Message::StructMessage m(p, Message::EPayloadType::System);

			manager->PushMessageSendQueue(m, sessionNumber);
		}
		else
		{
			Message::MessagePayload p;
			p.system = Message::SystemMessage(Message::ESystemMessageType::Login, "Fail");
			Message::StructMessage m(p, Message::EPayloadType::System);

			manager->PushMessageSendQueue(m, sessionNumber);
		}
		break;
		
	case Message::ESystemMessageType::HeartBeat:
		//std::cout << "Hello, heartbeat!!\n";
		manager->LastResponse = message->Header.TimeStamp;
		break;
	}
}

void MessageHandler::HandleChattingMessage(SocketManager*& manager, int sessionNumber, Message::StructMessage* message)
{
	std::cout << "WOW!!\n";
	std::cout << message->Payload.chatting.Message << std::endl;
	switch (message->Payload.chatting.Type)
	{
	case Message::EChattingMessageType::All:
		BroadcastChatting(message);
	}
}

void MessageHandler::HandleFriendMessage(SocketManager*& manager, int sessionNumber, Message::StructMessage* message)
{
	std::cout << "LOL!!\n";
	std::cout << "from: " << message->Payload.friendmsg.Sender << " target: "
		<< message->Payload.friendmsg.Target << std::endl;
}

void MessageHandler::DisconnectClient(SocketManager* manager, std::string name, int session)
{
	LoginLock.lock();
	if (LoginedClients.find(name) != LoginedClients.end())
	{
		if (LoginedClients.at(name).first == manager && LoginedClients.at(name).second == session)
		{
			LoginedClients.erase(name);
			std::cout << "Client " << name << " disconnected!!\n";
		}
	}
	LoginLock.unlock();
}

void MessageHandler::HeartBeatSeeker(MessageHandler* handler)
{
	std::cout << "HeartBeatSeeker working\n";
	while (true)
	{
		Sleep(1000); // 1 second
		auto now = std::chrono::system_clock::now();
		auto millis = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		//std::cout << "Wake up!!\n";
		for (auto& [name, info] : handler->LoginedClients)
		{
			auto& [manager, session] = info;
			//std::cout << manager->LastResponse << std::endl
			//	<< millis << std::endl;
			if (millis - manager->LastResponse> 1000 * 10)
			{
				manager->Disconnect();
			}
		}
	}
}

void MessageHandler::BroadcastChatting(Message::StructMessage* message)
{
	Message::MessagePayload p;
	p.chatting = Message::ChattingMessage(Message::EChattingMessageType::All, message->Payload.chatting.Sender, "", message->Payload.chatting.Message);
	Message::StructMessage m(p, Message::EPayloadType::Chatting);
	for (auto& [name, p] : LoginedClients)
	{
		if (name == message->Payload.chatting.Sender)
		{
			continue;
		}
		auto& [manager, session] = p;
		manager->PushMessageSendQueue(m, session);
	}
}