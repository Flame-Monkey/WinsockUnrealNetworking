#pragma once
#include <string>
#include "MessagePayloadBase.h"

namespace Message
{
	enum class EChattingMessageType : unsigned char
	{
		All = 0,
		Whisper = 1,
		Friend = 2,
		Group = 3,
		Admin = 4,
		None = 0xff,
	};


	// 151 bytes
	#pragma pack(push, 1)
	struct ChattingMessage : public MessagePayloadBase
	{
	public:
		EChattingMessageType Type;	// 1 byte
		char Sender[25];			// 25 bytes
		char Receiver[25]; 			// 25 bytes
		char Message[100];			// 100 bytes

		ChattingMessage(std::string payload = "");
		~ChattingMessage();
		unsigned int Size();
		void Serialize(char* data);
		void Deserialize(const char* data, unsigned int size);
	};
	#pragma pack(pop)
}