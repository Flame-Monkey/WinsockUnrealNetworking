#include "ChattingClient.h"
#include "ChattingMessage.h"
#include <iostream>
#include <chrono>
#include <thread>

ChattingClient::ChattingClient() :
	WSAData(), Socket(0), CompletePort(0), Addr(), ReceivedMessageQueue(), SendMessageQueue(),
	RecvBuffer(nullptr), RecvContext(), SendContext(),
	MessageBufferManager(192, 10'000'000, 1), MessageManager(&MessageBufferManager, 1)
{
}

void ChattingClient::Init()
{
	MessageBufferManager.Init();
	MessageManager.Init();

	//MessageBufferManager.TestBufferwrite();

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
}

void ChattingClient::SendWorkerThread(ChattingClient* client)
{
	char* buffer;
	unsigned long length;
	while (true)
	{
		std::unique_lock<std::mutex> queueLock(client->SendQueueMutex);

		client->SendQueueCV.wait(queueLock, [client] {
			return !client->SendMessageQueue.empty();
			});


		for (; client->SendIndex < 10 && !client->SendMessageQueue.empty(); client->SendIndex++)
		{
			Message::StructMessage m = client->SendMessageQueue.front();
			client->SendMessageQueue.pop();
			client->MessageManager.GetSendBuffer(m, buffer, length);

			client->SendContext.DataBuf[client->SendIndex].buf = buffer;
			client->SendContext.DataBuf[client->SendIndex].len = length;
			client->BytesToTransfer += length;
		}
		queueLock.unlock();

		if (client->SendIndex > 0)
		{
			std::unique_lock<std::mutex> flowLock(client->SendFlowMutex);

			client->SendFlowCV.wait(flowLock, [client] {
				return !client->IsSending;
				});

			client->IsSending = true;
			if (WSASend(client->Socket, client->SendContext.DataBuf, client->SendIndex, NULL,
				0, &client->SendContext.Overlapped, NULL) == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
			{
				std::cerr << "WSASend() Error: " << WSAGetLastError() << std::endl;
				exit(-1);
			}
			if (client->SendIndex > 1)
			{
				std::cout << "multi message sended!!\n";
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

	IOCPworker = std::thread(IOCPWorkerThread, this);
	SendWorker = std::thread(SendWorkerThread, this);

	if (WSARecv(Socket, RecvContext.DataBuf, 1, NULL, (LPDWORD)&RecvContext.Flags, &RecvContext.Overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			std::cerr << "WSARecv() Error: " << WSAGetLastError() << std::endl;
			exit(-1);
		}
	}
}

void ChattingClient::IOCPWorkerThread(ChattingClient* client)
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
			client->CompleteSend(bytesTransferred);
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

void ChattingClient::CompleteRecv(int transferred)
{
	for (int i = 0; i < transferred; ++i)
	{
		printf("%02x", RecvContext.DataBuf->buf[i]);
	}
	if (MessageManager.TransferByte(RecvBuffer, transferred))
	{
		Message::StructMessage* message;
		while (MessageManager.GetQueuedMessage(message))
		{
			ReceivedMessageQueue.push(message);
			if (message->Type == Message::EPayloadType::System)
			{
				std::cout << "WOW!!\n";
				if (message->Payload.system.Type == Message::ESystemMessageType::Login)
				{
					std::cout << message->Payload.system.Payload << std::endl;;
				}
			}
		}
	}
}

void ChattingClient::CompleteSend(unsigned int bytesTransferred)
{
	std::unique_lock<std::mutex> a(SendFlowMutex);
	if (BytesToTransfer == bytesTransferred)
	{
		if (BufferForRelease)
		{
			MessageManager.ReleaseMessageBuffer(BufferForRelease);
			BufferForRelease = nullptr;
		}
		unsigned int count = 0;
		for (int i = 0; i < 10 && count < bytesTransferred; ++i)
		{
			MessageManager.ReleaseMessageBuffer(SendContext.DataBuf[i].buf);
			count += SendContext.DataBuf[i].len;
		}
		BytesToTransfer = 0;
		SendIndex = 0;
		BufferForRelease = nullptr;
	}
	else
	{
		std::cout << "!@#!@@#!@#!@#!@!@!!!!!!!!!!!!#@$$$$$$$$$@\n";
		for (int i = 0; i < 10; ++i)
		{
			if (SendContext.DataBuf[i].len < bytesTransferred)
			{
				BytesToTransfer -= SendContext.DataBuf[i].len;
				bytesTransferred -= SendContext.DataBuf[i].len;
				if (i == 0 && BufferForRelease)
				{
					MessageManager.ReleaseMessageBuffer(BufferForRelease);
					BufferForRelease = nullptr;
				}
				MessageManager.ReleaseMessageBuffer(SendContext.DataBuf[i].buf);
			}
			else
			{
				if (i == 0 && !BufferForRelease)
				{
					BufferForRelease = SendContext.DataBuf[0].buf;
				}
				else if (1 != 0)
				{
					if (BufferForRelease)
					{
						MessageManager.ReleaseMessageBuffer(BufferForRelease);
					}
					BufferForRelease = SendContext.DataBuf[i].buf;
				}
				SendContext.DataBuf[i].buf = SendContext.DataBuf[i].buf + bytesTransferred;
				SendContext.DataBuf[i].len -= bytesTransferred;
				BytesToTransfer -= bytesTransferred;
				int count = 0;
				for (int j = 0; i < 10 && count < BytesToTransfer; ++j, ++i)
				{
					SendContext.DataBuf[j].buf = SendContext.DataBuf[i].buf;
					SendContext.DataBuf[j].len = SendContext.DataBuf[i].len;
					count += SendContext.DataBuf[j].len;
					SendIndex = j;
				}
				SendIndex++;
				break;
			}
		}
	}
	IsSending = false;
	SendFlowCV.notify_one();
}

void ChattingClient::Disconnect()
{
	shutdown(Socket, SD_BOTH);
	closesocket(Socket);
}

Message::StructMessage* ChattingClient::GetQueuedMessage()
{
	Message::StructMessage* ret = ReceivedMessageQueue.front();
	ReceivedMessageQueue.pop();
	return ret;
}

void ChattingClient::SendChat(std::string chat)
{
	Message::MessagePayload p;
	p.chatting = Message::ChattingMessage{ Message::EChattingMessageType::All, "", "", chat };
	Message::StructMessage m{ p, Message::EPayloadType::Chatting };

	AddMessageSendqueue(m);
}

void ChattingClient::SendFriendRequest(std::string target)
{
	Message::MessagePayload p;
	p.friendmsg = Message::FriendMessage{ Message::EFriendMessageType::Request, "foo", target };
	Message::StructMessage m{ p, Message::EPayloadType::Friend };

	AddMessageSendqueue(m);
}

void ChattingClient::Login(std::string name)
{
	Message::MessagePayload p;
	p.system = Message::SystemMessage(Message::ESystemMessageType::Login, name);
	Message::StructMessage m(p, Message::EPayloadType::System);

	AddMessageSendqueue(m);
}

void ChattingClient::AddMessageSendqueue(Message::StructMessage message)
{
	auto a = std::unique_lock<std::mutex>(SendQueueMutex);
	SendMessageQueue.push(message);
	SendQueueCV.notify_one();
}

void ChattingClient::PrintStatus()
{
	std::cout << "\nChattingClient PrintStatus\n";
	char address[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &Addr.sin_addr, address, sizeof(address));
	std::cout << "Connected to: " << address << ":" << ntohs(Addr.sin_port) << std::endl;

	MessageManager.PrintStatus();
}