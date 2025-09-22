#pragma once

#include "MessagePayloadBase.h"

namespace Message
{
	enum class ESystemMessageType : unsigned char
	{
		Login = 0,
		None = 0xff,
	};

	// 15 bytes
	#pragma pack(push, 1)
	struct SystemMessage : public MessagePayloadBase
	{
	public:

		ESystemMessageType Type;	// 1 byte
		char Payload[200];			// 200 bytes
		SystemMessage();
		~SystemMessage();
		unsigned int Size();
		void Serialize(char* data);
		void Deserialize(const char* data, unsigned int size);
	};
#pragma pack(pop)
}