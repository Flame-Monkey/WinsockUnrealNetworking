#include "SystemMessage.h"

#include <assert.h>
Message::SystemMessage::SystemMessage(Message::ESystemMessageType type, std::string payload) :
	Type(type), Payload('\0')
{
	assert(sizeof(SystemMessage) == Message::SystemMessage::SystemMessageSize);

	int index = 0;
	while (index < payload.length() && index < SystemMessage::PayloadSize)
	{
		Payload[index] = payload.at(index);
		index++;
	}
}