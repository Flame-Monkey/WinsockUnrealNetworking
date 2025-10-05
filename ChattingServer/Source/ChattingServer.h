#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

#include "StructMessage.h"
#include "BufferManager.h"
#include "SocketManager.h"
#include "MessageHandler.h"

class ChattingServer;
class SocketManager;

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
	SocketManager* Manager;
	ESocketOperation LastOp;
	SOCKET Socket;
};

class ChattingServer
{
private:
	static DWORD WINAPI IOCPWorkerThread(LPVOID);

	WSADATA WSAData;
	HANDLE CompletionPort;
	sockaddr_in ServerAddr;

	SOCKET ListenSocket;
	SOCKET AcceptSocket;
	SocketContext* AcceptContext;

	static LPFN_ACCEPTEX lpfnAcceptEx;
	static LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockAddrs;


	Message::BufferManager* MessageBufferManager;
	unsigned int MessageBufferSize;
	unsigned int MaxMessageCount;
	unsigned long long ChannelSize;
	SocketManager** SocketManagerPool; // array stack
	std::mutex* ConnectionLock;
	unsigned long long MaxConnection;
	unsigned long long CurrentConnectionCount;

	HANDLE* WorkerThreadPool;
	unsigned long MaxWorkerThread;

	MessageHandler* MessageHandler;

	void CompleteAccept();
	void CompleteRecv(SocketContext* context, int bytesTransffered);
	void CompleteSend(SOCKET socket, SocketContext* context);
	void CloseConnect(SocketContext* context);

public:
	ChattingServer(sockaddr_in serverAddr,
		unsigned int messageBufferSize = 192, unsigned int maxMessageCount = 1'000'000, unsigned long long channelSize = 10,
		unsigned long long maxConnection = 10'000, unsigned long maxWorkerThread = 20);
	~ChattingServer();

	void Init();
	void InitWSAFunc();
	void StartServer();
	void Stop();
	void StartAccept();
	void StartRecv(SocketContext* context);
	void Send();
	void PrintStatus();
};