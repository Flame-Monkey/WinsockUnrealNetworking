#include "ChattingMessage.h"
#include "FriendMessage.h"
#include <string>
#include <algorithm>

//Message::ChattingMessage::ChattingMessage(std::string payload) :
//	Type(EChattingMessageType::All),
//	Sender{ 0 },
//	Receiver{ 0 },
//	Message{ 0 }
//{
//	if (payload.length() <= 100)
//	{
//		std::copy(payload.c_str(), payload.c_str() + payload.length(), Message);
//	}
//}
//
//Message::ChattingMessage::~ChattingMessage()
//{
//}
//
//unsigned int Message::ChattingMessage::Size()
//{
//	return 1 + 25 + 25 + static_cast<unsigned int>((std::find(std::begin(Message), std::end(Message), '\0') - std::begin(Message)));
//}
//
//void Message::ChattingMessage::Serialize(char* data)
//{
//	data[0] = static_cast<char>(Type);
//	std::copy(Sender, Sender + 25, data + 1);
//	std::copy(Receiver, Receiver + 25, data + 1 + 25);
//
//	for (int i = 0; i < 100; i++)
//	{
//		data[1 + 25 + 25 + i] = Message[i];
//		if (Message[i] == '\0')
//			break;
//	}
//}
//
//void Message::ChattingMessage::Deserialize(const char* data, unsigned int size)
//{
//	std::copy(data, data + size, reinterpret_cast<char*>(this));
//}
