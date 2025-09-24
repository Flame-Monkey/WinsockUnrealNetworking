#pragma once

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
		ESystemMessageType Type;	// 1 byte
		char Payload[100];			// 100 bytes
	};
#pragma pack(pop)
}