#pragma once

#include "ChattingServer.h"
#include "MessageManager.h"
#include <queue>

class ChattingServer;
struct SocketContext;

class SocketManager
{
private:
	int Id;
	SOCKET Socket;
	ChattingServer* Server = nullptr;
	Message::BufferManager* MessageBufferManager = nullptr;
	Message::MessageManager MessageManager;

public:
	SocketContext* RecvContext;
	SocketContext* SendContext;
	char* RecvBuffer;
	char* SendBuffer;
	unsigned long BufferLength = 1000;
	std::queue<Message::StructMessage*> MessageQueue;

	SocketManager(ChattingServer* server, Message::BufferManager* bufferManager, int id);
	~SocketManager() = default;

	void Init();

	void SetSocket(SOCKET socket);
	void ProcessRecv(int Transffered);
};