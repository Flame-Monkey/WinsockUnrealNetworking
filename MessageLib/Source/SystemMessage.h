#pragma once

#include <string>

namespace Message
{
	enum class ESystemMessageType : unsigned char
	{
		Login = 0,
		None = 0xff,
	};

	// 101 bytes
	#pragma pack(push, 1)
	struct SystemMessage
	{
	public:
		ESystemMessageType Type;	// 1 byte
		char Payload[100];			// 100 bytes

		inline static int SystemMessageSize = 101;
		inline static int PayloadSize = 100;
		SystemMessage(ESystemMessageType type, std::string payload);
	};
#pragma pack(pop)
}