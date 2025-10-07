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

	SOCKET Socket;
	bool IsConnected = false;
	bool IsSending = false;
	bool IsInServerSendQueue = false;
	char* BufferForRelease;

	std::queue<Message::StructMessage> SendMessageQueue;
	std::mutex SendLock;
	std::condition_variable SendCV;
	int BytesToTransfer = 0;

	void Disconnect();
	void ReleaseSendBuffer();

public:
	SocketContext* RecvContext;
	SocketContext* SendContext;
	char* RecvBuffer;
	int SendIndex = 0;
	unsigned long BufferLength = 1000;

	SocketManager(ChattingServer* server, Message::BufferManager* bufferManager, int id, MessageHandler* handler);
	~SocketManager() = default;

	void Init();

	void SetSocket(SOCKET socket);
	void ProcessRecv(int Transffered);
	void PushMessageSendQueue(Message::StructMessage message);

	void ReleaseMessage(Message::StructMessage* message);
	void TrySend();
	void CompleteSend(int bytesTransferred);

	void Reset();

};