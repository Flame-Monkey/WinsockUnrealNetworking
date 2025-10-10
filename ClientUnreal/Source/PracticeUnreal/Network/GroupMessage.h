#pragma once

namespace Message
{
	enum class EGroupMessageType : unsigned char
	{
		Create = 0,
		Invite = 1,
		Kick = 2,
		Leave = 3,
		None = 0xff,
	};

	struct GroupMessage
	{
		EGroupMessageType Type;	// 1 byte
		char GroupName[25];		// 25 bytes
		char Sender[25];		// 25 bytes
		char Receiver[25]; 		// 25 bytes
	};
}