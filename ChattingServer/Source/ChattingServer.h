#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

#include "StructMessage.h"
#include "BufferManager.h"
#include "SocketManager.h"

class ChattingServer;

enum ESocketOperation
{
	Accept = 0,
	Recv = 1,
	Send = 2,
};

struct SocketContext
{
	OVERLAPPED Overlapped;
	ChattingServer* MyServer; // 이거 필요함?
	ESocketOperation LastOp;
	SOCKET Socket;
	WSABUF* DataBuf;
	int ImmediatelyReceivedBytes; // Won't use WSAGetOverlappedResult
	int Flags;
};

class ChattingServer
{
private:
	WSADATA WSAData;
	HANDLE Completionport;
	sockaddr_in SocketAddr;
	short PortNum;
	SOCKET ListenSocket;
	SOCKET AcceptSocket;
	SocketContext* AcceptContext;

	static LPFN_ACCEPTEX lpfnAcceptEx;
	static LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSocketAddrs;

	static DWORD WINAPI IOCPWorkerThread(LPVOID);

	Message::BufferManager* MessageBufferManager;
	unsigned long long MaxConnection;
	SocketManager* SocketManagerPool; // array stack

public:
	ChattingServer();
	~ChattingServer();

	void Init();
	void StartServer();
	void Stop();
	void Send();
	void PrintStatus();
};