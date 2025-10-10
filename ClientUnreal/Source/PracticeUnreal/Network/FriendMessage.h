#pragma once
#include <string>

namespace Message
{
	enum class EFriendMessageType : unsigned char
	{
		Request = 0,
		Accept = 1,
		Reject = 2,
		Delete = 3,
		None = 0xff,
	};

	// 51 bytes
	#pragma pack(push, 1)
	struct FriendMessage
	{
		EFriendMessageType Type;	// 1 byte
		char Sender[25];			// 25 bytes
		char Target[25]; 			// 25 bytes

		inline static int FriendMessageSize = 51;
		inline static int SenderSize = 25;
		inline static int TargetSize = 25;

		FriendMessage(EFriendMessageType type, std::string sender, std::string target);
	};
	#pragma pack(pop)
}