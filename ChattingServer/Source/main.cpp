#include <iostream>
#include "ChattingServer.h"

int main()
{
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5000);

	ChattingServer server{addr};

	server.Init();
	server.StartServer();

	while (true) 
	{
		std::string input;
		std::getline(std::cin, input);
		if (input == "print")
		{
			server.PrintStatus();
			continue;
		}
	}

	return 0;
}