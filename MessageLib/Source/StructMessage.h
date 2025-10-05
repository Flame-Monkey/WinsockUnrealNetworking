#pragma once

#include "SystemMessage.h"
#include "ChattingMessage.h"
#include "FriendMessage.h"
#include "GroupMessage.h"

namespace Message
{
enum class EMessageType : unsigned char
{
	StructMessage = 0,
};

// 14 bytes
#pragma pack(push, 1)
struct MessageHeader
{
	EMessageType Type;			// 1 byte
	unsigned char Flags;		// 1 byte
	unsigned int PayloadLength;	// 4 bytes
	long long TimeStamp;		// 8 bytes
};
#pragma pack(pop)

enum class EPayloadType : unsigned char
{
	System = 0,
	Chatting = 1,
	Friend = 2,
	Group = 3,
	None = 0xff,
};

// Size determined by the largest struct
union MessagePayload
{
	SystemMessage system;	// 101 bytes
	ChattingMessage chatting;	// 151 bytes
	FriendMessage frendmsg;		// 51 bytes
	GroupMessage group;			// 76 bytes

	MessagePayload() : system() {}
	~MessagePayload() {}

	unsigned int Size(EPayloadType type);
};

// 166 bytes
#pragma pack(push, 1)
struct StructMessage
{
public:
	MessageHeader Header; // 14 bytes
	EPayloadType Type; 	// 1 byte
	MessagePayload Payload;	// 151 bytes

	StructMessage(MessagePayload payload, EPayloadType type);

	unsigned int Size();
};
#pragma pack(pop)
}