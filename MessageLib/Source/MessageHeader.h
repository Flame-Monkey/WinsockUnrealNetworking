#pragma once 

#include <chrono>

namespace Message
{
	// 1 byte
	enum EMessageType : unsigned char
	{
		System = 0,
		Login = 1,
		Chatting = 2,
	};

	// 14 bytes
	#pragma pack(push, 1)
	struct MessageHeader
	{
		long long TimeStamp;	// 8 bytes
		unsigned int Length;	// 4 bytes
		EMessageType Type;		// 1 byte
		unsigned char Falgs;	// 1 byte
	};
	#pragma pack(pop)
}
