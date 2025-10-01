#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include "MessageManager.h"
#include <string>
#include <algorithm>

class ChattingClient
{
private:
	WSADATA WSAData;
	SOCKET Socket;
	sockaddr_in Addr;
	char *SendBuffer;
	bool IsLogin = false;
	std::queue<Message::StructMessage> MessageQueue;

public:
	ChattingClient();
	void Init();
	void Connect(std::string ipaddress, short portnum);
	void Disconnect();
	Message::StructMessage GetQueuedMessage();
	void SendChat(std::string chat);
};