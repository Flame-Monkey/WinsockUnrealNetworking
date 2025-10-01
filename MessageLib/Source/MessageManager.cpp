#include "MessageManager.h"
#include <iostream>

Message::MessageManager::MessageManager(BufferManager* manager, int id):
	MessageBufferManager(manager),
    ManagerID(id),
	TransferState(ETransferState::Header),
	MessageQueue() ,
    BufferDataLength(0),
    QueueMutex()
{
}

Message::MessageManager::~MessageManager()
{
    while (!MessageQueue.empty())
    {
        ReleaseMessageBuffer(MessageQueue.front());
        MessageQueue.pop();
    }
}

void Message::MessageManager::Init()
{
    if (!MessageBufferManager->GetMessageBuffer(Buffer, BufferSize, ManagerID))
    {
        std::cerr << "MessageManager::MessageManager() - Failed to get message buffer from BufferManager." << std::endl;
        exit(1);
    }
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
    unsigned int transfer = reinterpret_cast<MessageHeader*>(Buffer)->PayloadLength
        - (BufferDataLength - sizeof(MessageHeader));
    transfer = size < transfer ? size : transfer;

    memcpy(Buffer + BufferDataLength, data, transfer);
    BufferDataLength += transfer;

    if (BufferDataLength == sizeof(MessageHeader) + reinterpret_cast<MessageHeader*>(Buffer)->PayloadLength)
    {
        QueueMutex.lock();
        MessageQueue.push(Buffer);
		QueueMutex.unlock();

        BufferDataLength = 0;
        TransferState = ETransferState::Header;

        if (!MessageBufferManager->GetMessageBuffer(Buffer, BufferSize, ManagerID))
        {
            std::cerr << "MessageManager::TransferByte() - Failed to get buffer.!!!!!!!!" << std::endl;
            std::cout << "Current Buffer Manager's avliable Message Buffer count: " << MessageBufferManager->GetAvailableBufferCount() 
                << std::endl;
        }
    }

    return transfer;
}

bool Message::MessageManager::GetQueuedMessage(StructMessage*& outMessage)
{
	QueueMutex.lock();
    if (MessageQueue.empty())
    {
        QueueMutex.unlock();
        return false;
    }
    char* message = MessageQueue.front();
    MessageQueue.pop();
	QueueMutex.unlock();

    outMessage = (StructMessage*)message;
    return true;
}

void Message::MessageManager::ReleaseMessageBuffer(char* buffer)
{
    MessageBufferManager->ReleaseMessageBuffer(buffer);
}

bool Message::MessageManager::GetSendBuffer(MessagePayload payload, EPayloadType type, char*& outMessageBuffer, unsigned long long& outMessageLength)
{
    unsigned int outSize;
    if (!MessageBufferManager->GetMessageBuffer(outMessageBuffer, outSize, ManagerID)
        || outSize < sizeof(StructMessage))
    {
		std::cerr << "MessageManager::GetSendBuffer() - Failed to get send buffer from BufferManager, or size error." << std::endl;
        return false;
    }



    return true;
}