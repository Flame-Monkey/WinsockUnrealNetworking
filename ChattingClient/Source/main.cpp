#include <iostream>
#include "ChattingClient.h"
#include "StructMessage.h"
#include "BufferManager.h"	
#include <chrono>

int main()
{
	static_assert(sizeof(Message::MessageHeader) == 14, "MessageHeader size is not 14 bytes");
	static_assert(sizeof(Message::ChattingMessage) == 151, "ChattingMessage size is not 151 bytes");
	static_assert(sizeof(Message::FriendMessage) == 51, "FriendMessage size is not 51 bytes");

	auto now = std::chrono::system_clock::now();

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch()
	).count();

	std::cout << "Unix timestamp (ms): " << ms << std::endl;

	ChattingClient client{};
	client.Init();
	client.Connect("127.0.0.1", 5000);
	std::string input;
	std::cout << "Input your name" << std::endl;
	std::cin >> input;
	if (client.Login(input))
	{
		std::cout << "Login Seccess!!" << std::endl;
	}
	else
	{
		std::cout << "Login Failed" << std::endl;
		exit(-1);
	}

	std::cin.ignore();

	while (true)
	{
		std::getline(std::cin, input);
		if (input == "print")
		{
			client.PrintStatus();
			continue;
		}
		if (!std::cin.eof())
		{
			client.SendChat(input);
			//client.SendChat(input);
			//client.SendFriendRequest("jinho");
		}
		else
		{
			break;
		}
		client.Heartbeat();
	}
	return 0;
}