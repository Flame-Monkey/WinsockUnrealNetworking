#pragma once

#include "ChattingServer.h"
#include "MessageManager.h"
#include "MessageHandler.h"
#include <queue>

class ChattingServer;
struct SocketContext;
class MessageHandler;

class SocketManager
{
private:
	int Id;
	SOCKET Socket;
	ChattingServer* Server = nullptr;
	Message::BufferManager* MessageBufferManager = nullptr;
	Message::MessageManager MessageManager;
	bool IsSending = false;

	MessageHandler* Handler;

public:
	SocketContext* RecvContext;
	SocketContext* SendContext;
	char* RecvBuffer;
	char* SendBuffer;
	unsigned long BufferLength = 1000;

	SocketManager(ChattingServer* server, Message::BufferManager* bufferManager, int id, MessageHandler* handler);
	~SocketManager() = default;

	void Init();

	void SetSocket(SOCKET socket);
	void ProcessRecv(int Transffered);

	void ReleaseMessage(Message::StructMessage* message);
};