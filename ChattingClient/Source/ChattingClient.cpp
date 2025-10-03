#include "ChattingClient.h"
#include "ChattingMessage.h"
#include <iostream>
#include <chrono>
#include <thread>

ChattingClient::ChattingClient() :
	WSAData(), Socket(0), CompletePort(0), Addr(), ReceivedMessageQueue(), SendMessageQueue(), SendLock(),
	SendBuffer(nullptr), RecvBuffer(nullptr), RecvContext(), SendContext(),
	MessageBufferManager(192, 10'000'000, 1), MessageManager(&MessageBufferManager, 1)
{
}

void ChattingClient::Init()
{
	MessageBufferManager.Init();
	MessageManager.Init();

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
	RecvBuffer = new char[1000];

	CompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	RecvContext.DataBuf = new WSABUF;
	RecvContext.DataBuf->buf = RecvBuffer;
	RecvContext.DataBuf->len = 1000;
	RecvContext.LastOp = ESocketOperation::Recv;

	SendContext.DataBuf = new WSABUF[10];
	SendContext.LastOp = ESocketOperation::Send;

	std::cout << "re" << SendMessageQueue.size();
}

void ChattingClient::SendWorker(void* p)
{
	char* buffer;
	unsigned long length;
	int count = 0;
	ChattingClient* client = ((ChattingClient*)p);
	while (true)
	{
		if (!client->SendMessageQueue.empty())
		{
			//std::cout << client->SendMessageQueue.size();
			std::cout << client->SendMessageQueue.empty();
			count = 0;
			client->SendLock.lock();
			for (int i = 0; i < 10 && !client->SendMessageQueue.empty(); ++i)
			{
				Message::StructMessage m = client->SendMessageQueue.front();
				client->SendMessageQueue.pop();
				std::cout << "!!!\n";
				client->MessageManager.GetSendBuffer(m, buffer, length);
				std::cout << "???\n";

				client->SendContext.DataBuf[i].buf = buffer;
				client->SendContext.DataBuf[i].len = length;
				count++;
			}
			if (WSASend(client->Socket, client->SendContext.DataBuf, count, NULL,
				0, &client->SendContext.Overlapped, NULL) == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
			{
				std::cerr << "WSASend() Error:asd " << WSAGetLastError() << std::endl;
				exit(-1);
			}
		}
	}
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

	_SendWorker = std::thread(SendWorker, this);

	worker = std::thread(WorkerThread, this);
}

void ChattingClient::WorkerThread(ChattingClient* client)
{
	int bytesTransferred;
	SocketContext* context = nullptr;
	LPWSAOVERLAPPED lpOverlapped = NULL;
	SOCKET socket;

	while (true)
	{
		GetQueuedCompletionStatus(client->CompletePort, (LPDWORD)&bytesTransferred, (PULONG_PTR)&socket, &lpOverlapped, INFINITE);
		context = (SocketContext*)lpOverlapped;

		switch (context->LastOp)
		{
		case ESocketOperation::Recv:
			std::cout << "Process receive\n";
			client->CompleteRecv(bytesTransferred);
			break;
		case ESocketOperation::Send:
			std::cout << "Process send\n";
			client->CompleteSend();
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
	this->SendLock.unlock();
}

void ChattingClient::Disconnect()
{
	shutdown(Socket, SD_BOTH);
	closesocket(Socket);
}

Message::StructMessage ChattingClient::GetQueuedMessage()
{
	Message::StructMessage ret = ReceivedMessageQueue.front();
	ReceivedMessageQueue.pop();
	return ret;
}

void ChattingClient::SendChat(std::string chat)
{
	std::cout << "fuck worlld\n";
	Message::MessagePayload p;
	p.chatting = Message::ChattingMessage{ Message::EChattingMessageType::All, "", "", chat };
	Message::StructMessage m{ p, Message::EPayloadType::Chatting };

	SendMessageQueue.push(m);
}