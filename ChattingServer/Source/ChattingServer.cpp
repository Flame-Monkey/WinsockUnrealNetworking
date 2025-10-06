#include <iostream>
#include "ChattingServer.h"

LPFN_ACCEPTEX ChattingServer::lpfnAcceptEx = 0;
LPFN_GETACCEPTEXSOCKADDRS ChattingServer::lpfnGetAcceptExSockAddrs = 0;

DWORD __stdcall ChattingServer::IOCPWorkerThread(LPVOID serverInstance)
{
	ChattingServer* server = ((ChattingServer*)serverInstance);
	int bytesTransferred;
	SocketManager* manager = nullptr;
	SocketContext* context = nullptr;
	LPWSAOVERLAPPED lpOverlapped = NULL;

	while (true)
	{
		GetQueuedCompletionStatus(server->CompletionPort, (LPDWORD)&bytesTransferred, (PULONG_PTR)&manager, &lpOverlapped, INFINITE);
		context = (SocketContext*)lpOverlapped;

		switch (context->LastOp)
		{
		case ESocketOperation::Accept:
			std::cout << "Process accept\n";
			server->CompleteAccept();
			break;
		case ESocketOperation::Recv:
			std::cout << "Process receive\n";
			server->CompleteRecv(context, bytesTransferred);
			break;
		case ESocketOperation::Send:
			std::cout << "Process send\n";
			server->CompleteSend(context, bytesTransferred);
			break;
		default:
			std::cout << "Unknown Operation" << std::endl;
		}
	}
}

void ChattingServer::StartAccept()
{
	AcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	AcceptContext->LastOp = ESocketOperation::Accept;
	ZeroMemory(&AcceptContext->Overlapped, sizeof(OVERLAPPED));

	if (!lpfnAcceptEx(ListenSocket, AcceptSocket, AcceptContext->DataBuf->buf,
		0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &AcceptContext->RecvLength, &AcceptContext->Overlapped))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			std::cerr << "AcceptEx Error: " << WSAGetLastError() << std::endl;
			closesocket(AcceptSocket);
			exit(-1);
		}
	}
}

void ChattingServer::CompleteAccept()
{
	if (AcceptSocket == INVALID_SOCKET)
	{
		std::cerr << "Accept Socket Invalid" << std::endl;
		return;
	}
	std::cout << AcceptSocket << std::endl;

	sockaddr* localAddr = nullptr;
	sockaddr* remoteAddr = nullptr;
	int localLen = 0, remoteLen = 0;
	lpfnGetAcceptExSockAddrs(
		AcceptContext->DataBuf->buf,
		0,
		sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16,
		&localAddr,
		&localLen,
		&remoteAddr,
		&remoteLen
	);

	char ipstring[32];
	std::cout << "Client Connected. IPAddress: "
		<< inet_ntop(AF_INET, &((sockaddr_in*)remoteAddr)->sin_addr, ipstring, 20)
		<< " Port: " << ntohs(((SOCKADDR_IN*)remoteAddr)->sin_port) << std::endl;

	// Accept Process
	ConnectionLock->lock();
	if (CurrentConnectionCount >= MaxConnection)
	{
		closesocket(AcceptSocket);
	}
	SocketManager* manager = SocketManagerPool[CurrentConnectionCount];
	CurrentConnectionCount++;
	ConnectionLock->unlock();
	manager->SetSocket(AcceptSocket);

	if (CreateIoCompletionPort((HANDLE)AcceptSocket, CompletionPort, (ULONG_PTR)manager, 0) == NULL)
	{
		std::cerr << "CreateIoCompletionPort() Error: " << WSAGetLastError() << std::endl;
		return;
	}
	StartRecv(manager->RecvContext);
	StartAccept();

	return;
}

void ChattingServer::StartRecv(SocketContext* context)
{
	ZeroMemory(&context->Overlapped, sizeof(OVERLAPPED));

	if (WSARecv(context->Socket, context->DataBuf, 1, (LPDWORD)&context->ImmediatelyReceivedBytes,
		(LPDWORD)&context->Flags, &context->Overlapped, NULL) == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
	{
		std::cerr << "WSARecv() Error: " << WSAGetLastError() << std::endl;
		return;
	}
}

void ChattingServer::CompleteRecv(SocketContext* context, int bytesTransferred)
{
	if (bytesTransferred == 0)
	{
		return;
	}

	context->Manager->ProcessRecv(bytesTransferred);

	StartRecv(context);
}

void ChattingServer::CompleteSend(SocketContext* context, int bytesTransferred)
{
	context->Manager->CompleteSend(bytesTransferred);
}

void ChattingServer::CloseConnect(SocketContext* context)
{
}

ChattingServer::ChattingServer(
	sockaddr_in serverAddr, unsigned int messageBufferSize, unsigned int maxMessageCount,
	unsigned long long channelSize, unsigned long long maxConnection, unsigned long maxWorkerThread) :
	WSAData(), CompletionPort(), ServerAddr(serverAddr),
	ListenSocket(), AcceptSocket(), AcceptContext(), MessageBufferManager(nullptr),
	MessageBufferSize(messageBufferSize), MaxMessageCount(maxMessageCount), ChannelSize(channelSize),
	SocketManagerPool(nullptr), MaxConnection(maxConnection), CurrentConnectionCount(0),
	WorkerThreadPool(nullptr), MaxWorkerThread(maxWorkerThread), ConnectionLock(nullptr), Messagehandler(nullptr),
	SendQueue(), SenderThreadPool(nullptr)
{
}

ChattingServer::~ChattingServer()
{

}

void ChattingServer::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &WSAData))
	{
		std::cerr << "ChattingServer, Init(): WSAStartup Errer!!\n";

		exit(-1);
	}
	CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, (DWORD)MaxWorkerThread);

	MessageBufferManager = new Message::BufferManager(MessageBufferSize, MaxMessageCount, ChannelSize);
	MessageBufferManager->Init();

	Messagehandler = new MessageHandler(this, 10);
	Messagehandler->Init();

	SocketManagerPool = new SocketManager * [MaxConnection];
	for (int i = 0; i < MaxConnection; ++i)
	{
		SocketManagerPool[i] = new SocketManager{ this, MessageBufferManager, i + 1 , Messagehandler };
		SocketManagerPool[i]->Init();
	}
	ConnectionLock = new std::mutex();

	AcceptContext = new SocketContext;
	AcceptContext->DataBuf = new WSABUF;
	AcceptContext->DataBuf->buf = new char[(sizeof(SOCKADDR_IN) + 16) * 2];

	WorkerThreadPool = new HANDLE[MaxWorkerThread];

	SenderThreadPool = new std::thread*[MaxSenderThread];
}

void ChattingServer::InitWSAFunc()
{
	if (!ChattingServer::lpfnAcceptEx)
	{
		GUID guidAcceptEx = WSAID_ACCEPTEX;
		DWORD bytes = 0;

		if (WSAIoctl(ListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidAcceptEx, sizeof(guidAcceptEx),
			&lpfnAcceptEx, sizeof(lpfnAcceptEx), &bytes, NULL, NULL) == SOCKET_ERROR)
		{
			std::cerr << "WSAIoctl(AcceptEx) Error: " << WSAGetLastError() << std::endl;
			exit(-1);
		}
	}

	if (!ChattingServer::lpfnGetAcceptExSockAddrs)
	{
		GUID guidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
		DWORD dwBytes = 0;

		if (WSAIoctl(ListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidGetAcceptExSockaddrs, sizeof(guidGetAcceptExSockaddrs),
			&lpfnGetAcceptExSockAddrs, sizeof(lpfnGetAcceptExSockAddrs), &dwBytes, NULL, NULL) == SOCKET_ERROR)
		{
			printf("WSAIoctl for GetAcceptExSockaddrs failed: %d\n", WSAGetLastError());
			exit(-1);
		}
	}
}

void ChattingServer::StartServer()
{
	std::cout << "Server Starting..." << std::endl;

	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET)
	{
		std::cerr << "ChattingServer Init(): ListenSocket Init Error!!\n";

		exit(-1);
	}
	InitWSAFunc();

	if (bind(ListenSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		std::cerr << "ChattingServer StartServer, bind error " << ServerAddr.sin_port << ' ' << WSAGetLastError() << std::endl;

		exit(-1);
	}

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		exit(-1);
	}
	if (!CreateIoCompletionPort((HANDLE)ListenSocket, CompletionPort, (ULONG_PTR)AcceptContext, MaxWorkerThread))
	{
		exit(-1);
	}

	AcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (AcceptSocket == INVALID_SOCKET)
	{
		std::cerr << "Accept Socket Error: " << WSAGetLastError() << std::endl;
		exit(-1);
	}

	StartAccept();

	HANDLE hThread = INVALID_HANDLE_VALUE;

	for (unsigned long i = 0; i < MaxWorkerThread; ++i)
	{
		std::cout << "Create IOCP Worker Thread " << i << std::endl;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ChattingServer::IOCPWorkerThread, this, 0, NULL);
		WorkerThreadPool[i] = hThread;
	}
	for (unsigned long i = 0; i < MaxSenderThread; ++i)
	{
		std::cout << "Create sender thread #" << i << " \n";
		SenderThreadPool[i] = new std::thread(SendWorker, this);
	}

	std::cout << "Server Listening On port " << ntohs(ServerAddr.sin_port) << std::endl;

	Messagehandler->Start();
}

void ChattingServer::Stop()
{

}

void ChattingServer::Send(SocketManager* manager)
{
	ZeroMemory(&manager->SendContext->Overlapped, sizeof(OVERLAPPED));

	if (WSASend(manager->SendContext->Socket, manager->SendContext->DataBuf, manager->SendIndex, NULL,
		0, &manager->SendContext->Overlapped, NULL) == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
	{
		std::cerr << "WSASend() Error: " << WSAGetLastError() << std::endl;
		exit(-1);
	}
	else
	{
		std::cout << "Data Sended\n";
	}
}

void ChattingServer::PrintStatus()
{
	std::cout << "\nChattingServer PrintStatus\n"
		<< "MaxConnection(SocketManagerSize): " << MaxConnection << std::endl;
	MessageBufferManager->PrintStatus();
}

void ChattingServer::ReleaseMessage(Message::StructMessage* message)
{
	MessageBufferManager->ReleaseMessageBuffer((char*)message);
}

void ChattingServer::SendWorker(ChattingServer* server)
{
	SocketManager* manager;
	while (true)
	{
		auto lock = std::unique_lock<std::mutex>(server->SendLock);
		if (server->SendQueue.empty())
		{
			server->SendCV.wait(lock, [server] {
				return !server->SendQueue.empty();
				});
		}
		manager = server->SendQueue.front();
		server->SendQueue.pop();
		lock.unlock();

		manager->TrySend();
	}
}

void ChattingServer::SignalSend(SocketManager* manager)
{
	auto lock = std::unique_lock<std::mutex>(SendLock);
	SendQueue.push(manager);
	SendCV.notify_one();
}