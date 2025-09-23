#include "MessageManager.h"
#include <iostream>

Message::BufferManager Message::MessageManager::MessageBufferManager{};

Message::MessageManager::MessageManager(int id, unsigned int MessageBufferSize, unsigned int ManagerNum):
    ManagerID(id),
	TransferState(ETransferState::Header),
	MessageQueue() 
{
}

void Message::MessageManager::Init()
{
	MessageBufferManager.Init();

	if (!MessageBufferManager.GetMessageBuffer(Buffer, BufferSize, ManagerID % 10))
	{
		std::cerr << "MessageManager::Init() - Failed to get message buffer from BufferManager." << std::endl;
	}
}

Message::MessageManager::~MessageManager()
{
    while (!MessageQueue.empty())
    {
        ReleaseMessage(MessageQueue.front());
        MessageQueue.pop();
    }
	MessageBufferManager.ReleaseBufferManager();
}

bool Message::MessageManager::TransferByte(const char* data, unsigned int size)
{
    unsigned int offset = 0;

    while (offset < size)
    {
        switch (TransferState)
        {
        case ETransferState::Header:
            offset += TransferByteToHeader(data + offset, size - offset);
            break;
        case ETransferState::Payload:
            offset += TransferByteToPayload(data + offset, size - offset);
            break;
        }
    }

    return !MessageQueue.empty();
}

unsigned int Message::MessageManager::TransferByteToHeader(const char* data, unsigned int size)
{
    unsigned int transfer = sizeof(MessageHeader) - BufferDataLength;
    transfer = size < transfer ? size : transfer;

    memcpy(Buffer + BufferDataLength, data, transfer);
    BufferDataLength += transfer;

    if (BufferDataLength == sizeof(MessageHeader))
    {
        TransferState = ETransferState::Payload;
    }

    return transfer;
}

unsigned int Message::MessageManager::TransferByteToPayload(const char* data, unsigned int size)
{
    unsigned int transfer = reinterpret_cast<MessageHeader*>(Buffer)->Length
        - (BufferDataLength - sizeof(MessageHeader));
    transfer = size < transfer ? size : transfer;

    memcpy(Buffer + BufferDataLength, data, transfer);
    BufferDataLength += transfer;

    if (BufferDataLength == sizeof(MessageHeader) + reinterpret_cast<MessageHeader*>(Buffer)->Length)
    {
        MessageQueue.push(reinterpret_cast<Message*>(Buffer));

        BufferDataLength = 0;
        TransferState = ETransferState::Header;

        if (!MessageBufferManager.GetMessageBuffer(Buffer, BufferSize, ManagerID % 10))
        {
            std::cerr << "MessageManager::TransferByte() - Failed to get buffer." << std::endl;
        }
    }

    return transfer;
}

struct Message::Message* Message::MessageManager::GetMessage()
{
    Message* message = MessageQueue.front();

    MessageQueue.pop();
	return message;
}

void Message::MessageManager::ReleaseMessage(Message* message)
{
    MessageBufferManager.ReleaseMessageBuffer(reinterpret_cast<char*>(message));
}
