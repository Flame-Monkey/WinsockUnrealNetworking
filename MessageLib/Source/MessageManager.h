#pragma once

#include "Message.h"

namespace Message
{
	class MessageManager
	{
	public:
		MessageManager();
		~MessageManager();

		bool TransferByte(const char* data, size_t size);
		struct Message* GetMessage();
	};
}