#pragma once

#include "Message.h"
#include "BufferManager.h"

namespace Message
{
	class MessageManager
	{
	private:
		static BufferManager MessageBufferManager;
		char* Buffer = nullptr;
		unsigned int BufferSize = 0;
		unsigned int BufferOffset = 0;
		unsigned int BufferDataLength = 0;

	public:
		MessageManager(unsigned int MessageBufferSize = 0, unsigned int ManagerNum = 0);
		~MessageManager();

		void Init();
		bool TransferByte(const char* data, size_t size);
		struct Message* GetMessage();
	};
}