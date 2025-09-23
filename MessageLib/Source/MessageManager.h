#pragma once

#include <queue>
#include "Message.h"
#include "BufferManager.h"

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
		static BufferManager MessageBufferManager;
		int ManagerID = -1;

		ETransferState TransferState = ETransferState::Header;
		char* Buffer = nullptr;
		unsigned int BufferSize = 0;
		unsigned int BufferDataLength = 0;
		std::queue<struct Message*> MessageQueue;

		unsigned int TransferByteToHeader(const char* data, unsigned int size);
		unsigned int TransferByteToPayload(const char* data, unsigned int size);

	public:
		MessageManager(int id, unsigned int MessageBufferSize = 0, unsigned int ManagerNum = 0);
		~MessageManager();

		void Init();
		bool TransferByte(const char* data, unsigned int size);
		struct Message* GetMessage();
		void ReleaseMessage(struct Message* message);
	};
}