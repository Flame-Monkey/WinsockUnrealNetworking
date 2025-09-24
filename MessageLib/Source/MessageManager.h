#pragma once

#include <queue>
#include <mutex>
#include "BufferManager.h"
#include "StructMessage.h"

namespace Message
{
	enum class ETransferState : unsigned char
	{
		Header = 0,
		Payload = 1
	};

	class MessageManager
	{
	private:
		BufferManager* MessageBufferManager;
		int ManagerID = -1;
		std::mutex QueueMutex;

		ETransferState TransferState = ETransferState::Header;
		char* Buffer = nullptr;
		unsigned int BufferSize = 0;
		unsigned int BufferDataLength = 0;
		std::queue<char*> MessageQueue;

		unsigned int TransferByteToHeader(const char* data, unsigned int size);
		unsigned int TransferByteToPayload(const char* data, unsigned int size);
	public:
		MessageManager(BufferManager* manager, int id);
		~MessageManager();

		bool TransferByte(const char* data, unsigned int size);
		struct Message* GetMessage();
		void ReleaseMessageBuffer(char* buffer);

		bool GetSendBuffer(MessagePayload payload, EPayloadType type,char*& outMessageBuffer, unsigned long long &outMessageLength);
	};
}