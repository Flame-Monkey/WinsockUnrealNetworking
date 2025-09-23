#include <iostream>
#include "Message.h"

int main()
{
	static_assert(sizeof(Message::MessageHeader) == 14, "MessageHeader size is not 14 bytes");
	static_assert(sizeof(Message::ChattingMessage) == 151, "ChattingMessage size is not 151 bytes");	
	static_assert(sizeof(Message::FriendMessage) == 51, "FriendMessage size is not 51 bytes");

	Message::ChattingMessage chat{};
	chat.Message[0] = 'H';
	std::cout << chat.Size() << std::endl;

	std::string input;
	std::cin >> input;
	std::cout << input << std::endl;

	Message::MessageManager manager(0);
	manager.Init();
	while (true) {}
}