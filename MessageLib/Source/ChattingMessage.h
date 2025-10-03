#pragma once
#include <string>

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
	struct ChattingMessage
	{
	public:
		EChattingMessageType Type;	// 1 byte
		char Sender[25];			// 25 bytes
		char Receiver[25]; 			// 25 bytes
		char Message[100];			// 100 bytes

		inline static unsigned int ChattingMessageSize = 151;
		inline static int SenderSize = 25;
		inline static int ReceiverSize = 25;
		inline static int ChattingSize = 100;

		ChattingMessage(EChattingMessageType type, std::string sender, std::string receiver, 
			std::string message);
	};
	#pragma pack(pop)
}