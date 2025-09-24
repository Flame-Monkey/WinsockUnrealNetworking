#pragma once

#include "ChattingServer.h"

class SocketManager
{
private:
	char* RecvBuffer = nullptr;
	unsigned int RecvBufferSize = 0;
	ChattingServer* Server = nullptr;
public:
	SocketManager() = default;
	~SocketManager() = default;

	//void Init(ChattingServer* server);
};