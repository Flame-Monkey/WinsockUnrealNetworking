#include "StructMessage.h"	
#include <chrono>

namespace Message
{
	void StructMessage::SetBuffer(MessagePayload payload, EPayloadType type, char* buffer)
	{
		// Message Type
		buffer[0] = static_cast<char>(EMessageType::StructMessage);

		// flag
		buffer[1] = 0;

		// payload length
		unsigned long long payloadlength = GetPayloadLength(type);
		memcpy(&buffer[2], &payloadlength, 4);

		// milliunixtime
		auto now = std::chrono::system_clock::now();
		long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			now.time_since_epoch()
		).count();
		memcpy(&buffer[6], &ms, 8);

		// payload
		buffer[7] = static_cast<char>(type);
		memcpy(&buffer[8], &payload, payloadlength);
	}

	unsigned long long StructMessage::GetPayloadLength(EPayloadType type)
	{
		switch (type)
		{
		case EPayloadType::System:
			return sizeof(SystemMessage);
		case EPayloadType::Chatting:
			return sizeof(ChattingMessage);
		case EPayloadType::Friend:
			return sizeof(FriendMessage);
		case EPayloadType::Group:
			return sizeof(GroupMessage);
		default:
			return 0;
		}
	}
}

