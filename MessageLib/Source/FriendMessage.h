#pragma once

#include "MessagePayloadBase.h"

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
	struct FriendMessage : public MessagePayloadBase
	{
	public:
		EFriendMessageType Type;	// 1 byte
		char Sender[25];			// 25 bytes
		char Receiver[25]; 			// 25 bytes

		FriendMessage();
		~FriendMessage();
		unsigned int Size();
		void Serialize(char* data);
		void Deserialize(const char* data, unsigned int size);
	};
	#pragma pack(pop)
}