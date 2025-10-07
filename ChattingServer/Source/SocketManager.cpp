#include "SocketManager.h"
#include <iostream>

SocketManager::SocketManager(ChattingServer* server, Message::BufferManager* bufferManager, int id, MessageHandler* handler) :
	Id(id),
	Socket(INVALID_SOCKET),
	Server(server),
	MessageBufferManager(bufferManager), MessageManager(bufferManager, id),
	SendContext(nullptr), RecvContext(nullptr), RecvBuffer(nullptr), BufferForRelease(nullptr),
	Handler(handler), SendMessageQueue(), SendLock(), SendCV()
{
}

void SocketManager::Init()
{
	SendContext = new SocketContext;
	RecvContext = new SocketContext;
	RecvBuffer = new char[BufferLength];

	SendContext->Flags = 0;
	SendContext->ImmediatelyReceivedBytes = 0;
	SendContext->DataBuf = new WSABUF[10];
	SendContext->LastOp = ESocketOperation::Send;
	SendContext->Manager = this;

	RecvContext->Flags = 0;
	RecvContext->ImmediatelyReceivedBytes = 0;
	RecvContext->DataBuf = new WSABUF;
	RecvContext->DataBuf->buf = RecvBuffer;
	RecvContext->DataBuf->len = BufferLength;
	RecvContext->LastOp = ESocketOperation::Recv;
	RecvContext->Manager = this;

	MessageManager.Init();
}

void SocketManager::SetSocket(SOCKET socket)
{
	Socket = socket;
	RecvContext->Socket = socket;
	SendContext->Socket = socket;
	IsConnected = true;
}

void SocketManager::ProcessRecv(int transferred)
{
	for (int i = 0; i < transferred; ++i)
	{
		printf("%02x", RecvContext->DataBuf->buf[i]);
	}
	printf("\n");
	if (transferred == 0)
	{
		Disconnect();
		return;
	}
	if (MessageManager.TransferByte(RecvContext->DataBuf->buf, transferred))
	{
		Message::StructMessage* message;
		while (MessageManager.GetQueuedMessage(message))
		{
			std::cout << "Message Parse Completed!!\n";

			Handler->PushMessage(this, message);
		}
	}
}

void SocketManager::ReleaseMessage(Message::StructMessage* message)
{
	MessageManager.ReleaseMessageBuffer((char*)message);
}

void SocketManager::PushMessageSendQueue(Message::StructMessage message)
{
	auto a = std::unique_lock<std::mutex>(SendLock);
	SendMessageQueue.push(message);
	if (!IsInServerSendQueue)
	{
		Server->SignalSend(this);
		IsInServerSendQueue = true;
	}
}

void SocketManager::TrySend()
{
	std::cout << "SocketManager Try Send\n";
	auto lock = std::unique_lock<std::mutex>(SendLock);
	IsInServerSendQueue = false;
	if (SendMessageQueue.empty() || IsSending)
	{
		return;
	}
	char* buffer;
	unsigned long length;
	for (; SendIndex < 10 && !SendMessageQueue.empty(); ++SendIndex)
	{
		auto message = SendMessageQueue.front();
		SendMessageQueue.pop();
		MessageManager.GetSendBuffer(message, buffer, length);
		SendContext->DataBuf[SendIndex].buf = buffer;
		SendContext->DataBuf[SendIndex].len = length;

		BytesToTransfer += length;
	}
	std::cout << SendIndex << " Messages Sending\n";
	IsSending = true;
	Server->Send(this);
}

void SocketManager::CompleteSend(int bytesTransferred)
{
	std::unique_lock<std::mutex> a(SendLock);
	if (!IsConnected)
	{
		ReleaseSendBuffer();
		return;
	}
	if (BytesToTransfer == bytesTransferred)
	{
		int i = 0;
		if (BufferForRelease)
		{
			MessageManager.ReleaseMessageBuffer(BufferForRelease);
			BufferForRelease = nullptr;
			i = 1;
		}
		unsigned int count = 0;
		for (; i < 10 && count < bytesTransferred; ++i)
		{
			MessageManager.ReleaseMessageBuffer(SendContext->DataBuf[i].buf);
			count += SendContext->DataBuf[i].len;
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
			if (SendContext->DataBuf[i].len < bytesTransferred)
			{
				BytesToTransfer -= SendContext->DataBuf[i].len;
				bytesTransferred -= SendContext->DataBuf[i].len;
				if (i == 0 && BufferForRelease)
				{
					MessageManager.ReleaseMessageBuffer(BufferForRelease);
					BufferForRelease = nullptr;
					continue;
				}
				MessageManager.ReleaseMessageBuffer(SendContext->DataBuf[i].buf);
			}
			else
			{
				if (i == 0 && !BufferForRelease)
				{
					BufferForRelease = SendContext->DataBuf[0].buf;
				}
				else if (1 != 0)
				{
					BufferForRelease = SendContext->DataBuf[i].buf;
				}
				SendContext->DataBuf[i].buf = SendContext->DataBuf[i].buf + bytesTransferred;
				SendContext->DataBuf[i].len -= bytesTransferred;
				BytesToTransfer -= bytesTransferred;
				int count = 0;
				int j;
				for (j = 0; i < 10 && count < BytesToTransfer; ++j, ++i)
				{
					SendContext->DataBuf[j].buf = SendContext->DataBuf[i].buf;
					SendContext->DataBuf[j].len = SendContext->DataBuf[i].len;
					count += SendContext->DataBuf[j].len;
				}
				SendIndex = j;
				break;
			}
		}
	}
	IsSending = false;
	if (!SendMessageQueue.empty())
	{
		Server->SignalSend(this);
	}
}

void SocketManager::Disconnect()
{
	MessageManager.Reset();
	auto lock = std::unique_lock<std::mutex>(SendLock);
	ReleaseSendBuffer();
	while (!SendMessageQueue.empty())
	{
		SendMessageQueue.pop();
	}
	Server->Disconnect(this);
	Reset();
	std::cout << "Client Disconnected\n";
}

void SocketManager::Reset()
{
	IsSending = false;
	IsConnected = false;
	IsInServerSendQueue = false;
	BufferForRelease = nullptr;
	BytesToTransfer = 0;
}

void SocketManager::ReleaseSendBuffer()
{
	int i = 0;
	if (BufferForRelease)
	{
		MessageManager.ReleaseMessageBuffer(BufferForRelease);
		BufferForRelease = nullptr;
		++i;
	}
	for (; i < SendIndex; ++i)
	{
		MessageManager.ReleaseMessageBuffer(SendContext->DataBuf[i].buf);
	}
}