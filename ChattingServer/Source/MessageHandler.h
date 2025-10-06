#pragma once

#include "ChattingServer.h"
#include "SocketManager.h"
#include "StructMessage.h"
#include <thread>
#include <mutex>
#include <queue>
#include <map>

struct StructMessage;
class SocketManager;
class ChattingServer;

class MessageHandler
{
private:
	ChattingServer* Server;
	std::mutex HandlerLock;
	std::condition_variable HandlerCV;
	std::queue<std::pair<SocketManager*, Message::StructMessage*>> MessageQueue;

	std::map<std::string, SocketManager*> ConnectedClients;

	std::thread* ThreadPool;
	int MaxThread;

	void HandleSystemMessage(SocketManager*& manager, Message::StructMessage* message);
	void HandleChattingMessage(SocketManager*& manager, Message::StructMessage* message);
	void HandleFriendMessage(SocketManager*& manager, Message::StructMessage* message);


public:
	MessageHandler(ChattingServer* server, int);
	void Init();
	void Start();
	static void WorkerThread(MessageHandler* handler, int i);
	void PushMessage(SocketManager* manager, Message::StructMessage* message);
	void HandleMessage(SocketManager*& manager, Message::StructMessage* message);
};