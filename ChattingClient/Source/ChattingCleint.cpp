#include "ChattingClient.h"
#include <iostream>

ChattingClient::ChattingClient() :
	WSAData(), Socket(0), Addr(), MessageQueue(), SendBuffer(nullptr)
{

}

void ChattingClient::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &WSAData))
	{
		std::cerr << "ChattingClient, Init(): WSAStartup Errer!!\n";
		exit(-1);
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		std::cerr << "Socket Construct Error!!\n";
		exit(-1);
	}

	SendBuffer = new char[1000];
}

void ChattingClient::Connect(std::string ipaddress, short portnum)
{
	inet_pton(AF_INET, ipaddress.c_str(), &Addr.sin_addr.S_un);
	Addr.sin_port = htons(portnum);
	Addr.sin_family = AF_INET;

	if (connect(Socket, (sockaddr*)&Addr, sizeof(Addr)) == SOCKET_ERROR) {
		std::cerr << "Connect failed, error: " << WSAGetLastError() << "\n";
		return;
	}
}

void ChattingClient::Disconnect()
{
}

Message::StructMessage ChattingClient::GetQueuedMessage()
{
	return Message::StructMessage();
}

void ChattingClient::SendChat(std::string chat)
{
	std::copy(chat.c_str(), chat.c_str() + chat.length(), SendBuffer);
	send(Socket, SendBuffer, chat.length(), 0);
}