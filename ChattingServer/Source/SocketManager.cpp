#include "SocketManager.h"
#include <iostream>

SocketManager::SocketManager(ChattingServer* server, Message::BufferManager* bufferManager, int id, MessageHandler* handler):
	Id(id),
	Socket(INVALID_SOCKET),
	Server(server),
	MessageBufferManager(bufferManager), MessageManager(bufferManager, id),
	SendContext(nullptr), RecvContext(nullptr), SendBuffer(nullptr), RecvBuffer(nullptr), Handler(handler)
{
}

void SocketManager::Init()
{
	SendContext = new SocketContext;
	RecvContext = new SocketContext;
	SendBuffer = new char[BufferLength];
	RecvBuffer = new char[BufferLength];

	SendContext->Flags = 0;
	SendContext->ImmediatelyReceivedBytes = 0;
	SendContext->DataBuf = new WSABUF;
	SendContext->DataBuf->buf = SendBuffer;
	SendContext->DataBuf->len = BufferLength;
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
}

void SocketManager::ProcessRecv(int Transffered)
{
	for (int i = 0; i < Transffered; ++i)
	{
		printf("%02x", RecvContext->DataBuf->buf[i]);
	}
	printf("\n");

	if (MessageManager.TransferByte(RecvContext->DataBuf->buf, Transffered))
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