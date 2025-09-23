#include "BufferManager.h"

Message::BufferManager::BufferManager():
	bInit(false),
	Bufferpool(nullptr),
	MessageSize(192),
	MaxMessageSize(10'000'000),
	TotalBufferSize(MessageSize * MaxMessageSize)
{
}

void Message::BufferManager::Init(unsigned int MessageSize, unsigned int MaxMessageSize)
{
	if (bInit)
	{
		return;
	}
	this->MessageSize = MessageSize;
	this->MaxMessageSize = MaxMessageSize;
	this->TotalBufferSize = MessageSize * MaxMessageSize;
	this->Bufferpool = new char[TotalBufferSize];

	for (int i = 0; i < 10; ++i)
	{
		this->FreeBufferChannel[i].first = std::stack<char*>();
		this->FreeBufferChannel[i].second = new std::mutex();
	}
	
	for (int i = 0; i < MaxMessageSize; ++i)
	{
		int ChannelIndex = i % 10;
		char* BufferAddress = this->Bufferpool + (i * MessageSize);
		this->FreeBufferChannel[ChannelIndex].first.push(BufferAddress);
	}

	bInit = true;
}

Message::BufferManager::~BufferManager()
{
	delete Bufferpool;
	for (int i = 0; i < 10; ++i)
	{
		delete FreeBufferChannel[i].second;
	}
}

bool Message::BufferManager::GetMessageBuffer(char*& outBuffer, int& outBufferSize, int channelIndex)
{
	if (channelIndex < 0)
	{
		return GetBufferCommon(outBuffer, outBufferSize);
	}

	if(!GetBufferByChannel(channelIndex, outBuffer, outBufferSize))
	{
		return GetBufferCommon(outBuffer, outBufferSize);
	}
	return true;
}


bool Message::BufferManager::GetBufferCommon(char*& outBuffer, int& outBufferSize)
{
	for (int i = 0; i < 10; ++i)
	{
		if(GetBufferByChannel(i, outBuffer, outBufferSize))
		{
			return true;
		}
	}
	return false;
}

bool Message::BufferManager::GetBufferByChannel(int channelIndex, char*& outBuffer, int& outBufferSize)
{
	if (channelIndex < 0 || channelIndex >= 10)
	{
		return false;
	}

	auto& [stack, mtx] = FreeBufferChannel[channelIndex];
	mtx->lock();
	// stack이 비어있으면 어쩔, 세마포어로 교체(release에서 signal)
	outBuffer = stack.top();
	stack.pop();
	mtx->unlock();

	return true;
}

bool Message::BufferManager::ReleaseMessageBuffer(char* buffer)
{
	if (buffer == nullptr || buffer < Bufferpool || buffer >= Bufferpool + TotalBufferSize)
	{
		return false;
	}

	int index = (buffer - Bufferpool) / MessageSize;
	if (index >= MaxMessageSize || index < 0)
	{
		return false;
	}

	int channelIndex = index % 10;
	auto& [stack, mtx] = FreeBufferChannel[channelIndex];

	mtx->lock();
	stack.push(buffer);
	mtx->unlock();
	return true;
}