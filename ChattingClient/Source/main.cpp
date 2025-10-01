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

	//Message::ChattingMessage chat{};
	//chat.Message[0] = 'H';
	//std::cout << chat.Size() << std::endl;

	//std::string input;
	//std::cin >> input;
	//std::cout << input << std::endl;
	auto now = std::chrono::system_clock::now();

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch()
	).count();

	std::cout << "Unix timestamp (ms): " << ms << std::endl;

	Message::BufferManager bufferManager(192, 10'000'000, 10);
	bufferManager.Init();
	bufferManager.TestBufferwrite();

	ChattingClient client{};
	client.Init();
	client.Connect("127.0.0.1", 5000);
	std::string input;
	while (true) 
	{
		std::cin >> input;
		client.SendChat(input);
	}
	return 0;
}