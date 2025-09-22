#include "FriendMessage.h"
#include <algorithm>

Message::FriendMessage::FriendMessage() :
	Type(EFriendMessageType::None),
	Sender{ 0 },
	Receiver{ 0 }
{
}

Message::FriendMessage::~FriendMessage()
{
}

unsigned int Message::FriendMessage::Size()
{
	return 1 + 25 + 25;
}

void Message::FriendMessage::Serialize(char* data)
{
	data[0] = static_cast<char>(Type);
	std::copy(Sender, Sender + 25, data + 1);
	std::copy(Receiver, Receiver + 25, data + 1 + 25);
}

void Message::FriendMessage::Deserialize(const char* data, unsigned int size)
{
	std::copy(data, data + size, reinterpret_cast<char*>(this));
}