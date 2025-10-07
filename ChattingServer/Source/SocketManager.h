#pragma once

#include "ChattingServer.h"
#include "MessageManager.h"
#include "MessageHandler.h"
#include <queue>
#include <mutex>

class ChattingServer;
struct SocketContext;
class MessageHandler;

class SocketManager
{
private:
	int Id;
	ChattingServer* Server = nullptr;
	MessageHandler* Handler = nullptr;
	Message::BufferManager* MessageBufferManager = nullptr;
	Message::MessageManager MessageManager;

	bool IsConnected = false;
	bool IsSending = false;
	bool IsInServerSendQueue = false;
	char* BufferForRelease;

	std::queue<Message::StructMessage> SendMessageQueue;
	std::mutex SendLock;
	std::condition_variable SendCV;
	int BytesToTransfer = 0;

	std::string ClientName;

	void ReleaseSendBuffer();

public:
	SOCKET Socket;
	SocketContext* RecvContext;
	SocketContext* SendContext;
	char* RecvBuffer;
	int SendIndex = 0;
	unsigned long BufferLength = 1000;

	int SessionNumber = 0;
	long long LastResponse = 0LL;

	SocketManager(ChattingServer* server, Message::BufferManager* bufferManager, int id, MessageHandler* handler);
	~SocketManager() = default;

	void Init();

	void SetSocket(SOCKET socket);
	void Disconnect();
	void ProcessRecv(int Transffered);
	void PushMessageSendQueue(Message::StructMessage message, int session);

	void ReleaseMessage(Message::StructMessage* message);
	void TrySend();
	void CompleteSend(int bytesTransferred);

	void SetClientName(std::string name);

	void Reset();

};