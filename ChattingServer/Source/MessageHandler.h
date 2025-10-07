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
	std::queue<std::tuple<SocketManager*, Message::StructMessage*, int>> MessageQueue;

	std::mutex LoginLock;
	std::map<std::string, std::pair<SocketManager*, int>> LoginedClients;

	std::thread* ThreadPool;
	int MaxThread;
	std::thread* HeartBeatThread;

	void HandleSystemMessage(SocketManager*& manager, int sessionNumber, Message::StructMessage* message);
	void HandleChattingMessage(SocketManager*& manager, int sessionNumber, Message::StructMessage* message);
	void HandleFriendMessage(SocketManager*& manager, int sessionNumber, Message::StructMessage* message);

public:
	MessageHandler(ChattingServer* server, int);
	void Init();
	void Start();
	static void WorkerThread(MessageHandler* handler, int i);
	void PushMessage(SocketManager* manager, Message::StructMessage* message, int sessionNumber);
	void HandleMessage(SocketManager*& manager, int sessionNumber, Message::StructMessage* message);
	void DisconnectClient(SocketManager* manager, std::string name, int sessionNumber);

	static void HeartBeatSeeker(MessageHandler* handler);
};