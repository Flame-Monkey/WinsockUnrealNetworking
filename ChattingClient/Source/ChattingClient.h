#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include "MessageManager.h"
#include <string>
#include <algorithm>
#include <thread>

enum ESocketOperation
{
	Accept = 0,
	Recv = 1,
	Send = 2,
	Terminate = 0xffffffff,
};

struct SocketContext
{
	OVERLAPPED Overlapped;
	int ImmediatelyReceivedBytes;
	int Flags;
	unsigned long RecvLength;	// dont work
	WSABUF* DataBuf;
	ESocketOperation LastOp;
	SOCKET Socket;
};

class ChattingClient
{
private:
	WSADATA WSAData;
	HANDLE CompletePort;
	SOCKET Socket;
	SocketContext RecvContext;
	SocketContext SendContext;
	sockaddr_in Addr;
	char* SendBuffer;
	char* RecvBuffer;
	bool IsLogin = false;
	Message::BufferManager MessageBufferManager;
	Message::MessageManager MessageManager;
	std::queue<Message::StructMessage> MessageQueue;

	void WorkerThread();
	void StartRecv();
	void CompleteRecv(int);
	void CompleteSend();

public:
	ChattingClient();
	void Init();
	void Connect(std::string ipaddress, short portnum);
	void Disconnect();
	Message::StructMessage GetQueuedMessage();
	void SendChat(std::string chat);
};