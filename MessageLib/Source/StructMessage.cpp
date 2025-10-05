#include "StructMessage.h"	
#include <chrono>

namespace Message
{
	unsigned int MessagePayload::Size(EPayloadType type)
	{
		switch (type)
		{
		case EPayloadType::Chatting:
			return ChattingMessage::ChattingMessageSize;
		case EPayloadType::Friend:
			return FriendMessage::FriendMessageSize;
		case EPayloadType::System:
			return SystemMessage::SystemMessageSize;
		}
	}

	StructMessage::StructMessage(MessagePayload payload, EPayloadType type):
		Type(type), Payload(payload)
	{
		Header.Type = Message::EMessageType::StructMessage;
		Header.Flags = 0;
		Header.PayloadLength = payload.Size(type) + 1;
		auto now = std::chrono::system_clock::now();
		Header.TimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(
			now.time_since_epoch()
		).count();
	}

	unsigned int StructMessage::Size()
	{
		return 14 + 1 + Payload.Size(Type);
	}
}

