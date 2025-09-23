#include "MessageManager.h"

Message::BufferManager Message::MessageManager::MessageBufferManager{};

Message::MessageManager::MessageManager(unsigned int MessageBufferSize, unsigned int ManagerNum)
{
}
Message::MessageManager::~MessageManager()
{
}

void Message::MessageManager::Init()
{
	MessageBufferManager.Init();
}

bool Message::MessageManager::TransferByte(const char* data, size_t size)
{
	return false;
}
struct Message::Message* Message::MessageManager::GetMessage()
{
	return {};
}