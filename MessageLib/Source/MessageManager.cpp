#include "MessageManager.h"
#include <iostream>

Message::BufferManager Message::MessageManager::MessageBufferManager{};



Message::MessageManager::MessageManager(unsigned int MessageBufferSize, unsigned int ManagerNum):
	TransferState(ETransferState::Header),
	MessageQueue()
{
}


void Message::MessageManager::Init()
{
	MessageBufferManager.Init();

	if (!MessageBufferManager.GetMessageBuffer(Buffer, BufferSize, 0))
	{
		std::cerr << "MessageManager::Init() - Failed to get message buffer from BufferManager." << std::endl;
	}
}

Message::MessageManager::~MessageManager()
{
}

bool Message::MessageManager::TransferByte(const char* data, unsigned int size)
{
    unsigned int offset = 0;

    while (offset < size)
    {
        switch (TransferState)
        {
        case ETransferState::Header:
        {
            unsigned int need = sizeof(MessageHeader) - BufferDataLength;
            unsigned int transfer = std::min(size - offset, need);

            memcpy(Buffer + BufferDataLength, data + offset, transfer);
            BufferDataLength += transfer;
            offset += transfer;

            if (BufferDataLength == sizeof(MessageHeader))
            {
                TransferState = ETransferState::Payload;
            }
            break;
        }
        case ETransferState::Payload:
        {
            unsigned int need = reinterpret_cast<MessageHeader*>(Buffer)->Length
                - (BufferDataLength - sizeof(MessageHeader));
            unsigned int transfer = std::min(size - offset, need);

            memcpy(Buffer + BufferDataLength, data + offset, transfer);
            BufferDataLength += transfer;
            offset += transfer;

            if (BufferDataLength == sizeof(MessageHeader) + reinterpret_cast<MessageHeader*>(Buffer)->Length)
            {
                MessageQueue.push(reinterpret_cast<Message*>(Buffer));

                BufferOffset = 0;
                BufferDataLength = 0;
                TransferState = ETransferState::Header;

                if (!MessageBufferManager.GetMessageBuffer(Buffer, BufferSize, 1))
                {
                    std::cerr << "MessageManager::TransferByte() - Failed to get buffer." << std::endl;
                }
            }
            break;
        }
        }
    }

    return !MessageQueue.empty();
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
