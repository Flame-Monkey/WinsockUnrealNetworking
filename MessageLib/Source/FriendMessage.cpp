#include "FriendMessage.h"
#include <algorithm>
#include <assert.h>

Message::FriendMessage::FriendMessage(EFriendMessageType type, std::string sender, std::string target) :
	Type(type),
	Sender{ '\0'},
	Target{ '\0' }
{
	assert(sizeof(FriendMessage) == FriendMessage::FriendMessageSize);

	int index = 0;
	while (index < sender.length() && index < FriendMessage::SenderSize)
	{
		Sender[index] = sender.at(index);
		index++;
	}

	index = 0;
	while (index < target.length() && index < FriendMessage::TargetSize)
	{
		Target[index] = target.at(index);
		index++;
	}
}