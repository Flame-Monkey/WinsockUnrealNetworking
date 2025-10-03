#include "ChattingMessage.h"
#include "FriendMessage.h"
#include <string>
#include <algorithm>
#include <assert.h>
Message::ChattingMessage::ChattingMessage(EChattingMessageType type, std::string sender, std::string receiver, std::string message):
	Type{ type }, Sender{ '\0' }, Receiver{ '\0' }, Message{ '\0' }
{
	assert(sizeof(ChattingMessage) == ChattingMessageSize);

	int index = 0;
	while (index < sender.length() && index < SenderSize)
	{
		Sender[index] = sender.at(index);
		index++;
	}

	index = 0;
	while (index < receiver.length() && index < ReceiverSize)
	{
		Receiver[index] = receiver.at(index);
		index++;
	}

	index = 0;
	while (index < message.length() && index < ChattingSize)
	{
		Message[index] = message.at(index);
		index++;
	}
}
