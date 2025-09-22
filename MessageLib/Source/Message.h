#pragma once

#include "MessageHeader.h"
#include "MessagePayloadBase.h"
#include "MessageManager.h"
#include "ChattingMessage.h"
#include "FriendMessage.h"

namespace Message
{
	struct Message
	{
	public:
		MessageHeader header;		// 14 bytes
		MessagePayloadBase payload;
	};
}