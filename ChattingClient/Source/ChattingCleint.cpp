#include "ChattingClient.h"
#include "ChattingMessage.h"
#include <iostream>

ChattingClient::ChattingClient() :
	WSAData(), Socket(0), CompletePort(0), Addr(), MessageQueue(), 
	SendBuffer(nullptr), RecvBuffer(nullptr), RecvContext(), SendContext(),
	MessageBufferManager(192, 10'000'000, 1), MessageManager(&MessageBufferManager, 1)
{

}

void ChattingClient::Init()
{
	MessageBufferManager.Init();
	MessageBufferManager.TestBufferwrite();

	if (WSAStartup(MAKEWORD(2, 2), &WSAData))
	{
		std::cerr << "ChattingClient, Init(): WSAStartup Errer!!\n";
		exit(-1);
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		std::cerr << "Socket Construct Error!!\n";
		exit(-1);
	}

	SendBuffer = new char[1000];
	RecvBuffer = new char[1000];

	CompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	RecvContext.DataBuf = new WSABUF;
	RecvContext.DataBuf->buf = RecvBuffer;
	RecvContext.DataBuf->len = 1000;
	RecvContext.LastOp = ESocketOperation::Recv;

	SendContext.DataBuf = new WSABUF;
	SendContext.DataBuf->buf = SendBuffer;
	SendContext.DataBuf->len = 1000;
	SendContext.LastOp = ESocketOperation::Recv;
}

void ChattingClient::Connect(std::string ipaddress, short portnum)
{
	inet_pton(AF_INET, ipaddress.c_str(), &Addr.sin_addr.S_un);
	Addr.sin_port = htons(portnum);
	Addr.sin_family = AF_INET;

	if (connect(Socket, (sockaddr*)&Addr, sizeof(Addr)) == SOCKET_ERROR) {
		std::cerr << "Connect failed, error: " << WSAGetLastError() << "\n";
		return;
	}

	if (CreateIoCompletionPort((HANDLE)Socket, CompletePort, Socket, 0) == NULL)
	{
		std::cerr << "CreateIoCompletionPort() Error: " << WSAGetLastError() << std::endl;
		exit(-1);
	}
	StartRecv();
	std::thread worker([this]() {
		this->WorkerThread();
	});
}

void ChattingClient::WorkerThread()
{
	int bytesTransferred;
	SocketContext* context = nullptr;
	LPWSAOVERLAPPED lpOverlapped = NULL;
	SOCKET socket;

	while (true)
	{
		GetQueuedCompletionStatus(CompletePort, (LPDWORD)&bytesTransferred, (PULONG_PTR)&socket, &lpOverlapped, INFINITE);
		context = (SocketContext*)lpOverlapped;

		switch (context->LastOp)
		{
		case ESocketOperation::Recv:
			std::cout << "Process receive\n";
			this->CompleteRecv(bytesTransferred);
			break;
		case ESocketOperation::Send:
			std::cout << "Process send\n";
			CompleteSend();
			break;
		default:
			std::cout << "Unknown Operation" << std::endl;
		}
	}
}

void ChattingClient::StartRecv()
{
	ZeroMemory(&RecvContext.Overlapped, sizeof(OVERLAPPED));
	if (WSARecv(Socket, RecvContext.DataBuf, 1, (LPDWORD)&RecvContext.ImmediatelyReceivedBytes, (LPDWORD)&RecvContext.Flags,
		&RecvContext.Overlapped, NULL) == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
	{
		std::cerr << "WSARecv() Error: " << WSAGetLastError() << std::endl;
		return;
	}
}

void ChattingClient::CompleteRecv(int treansffered)
{

}

void ChattingClient::CompleteSend()
{

}

void ChattingClient::Disconnect()
{
	shutdown(Socket, SD_BOTH);
	closesocket(Socket);
}

Message::StructMessage ChattingClient::GetQueuedMessage()
{
	return Message::StructMessage();
}

void ChattingClient::SendChat(std::string chat)
{
	std::copy(chat.c_str(), chat.c_str() + chat.length(), SendBuffer);
	SendContext.DataBuf->len = chat.length();
	if (WSASend(Socket, SendContext.DataBuf, 1, NULL,
		0, &SendContext.Overlapped, NULL) == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
	{
				std::cerr << "WSASend() Error: " << WSAGetLastError() << std::endl;
		return;
	}

	Message::ChattingMessage chattingMessage;
}