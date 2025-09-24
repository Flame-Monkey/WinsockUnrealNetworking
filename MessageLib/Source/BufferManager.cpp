#include "BufferManager.h"
#include <iostream>
Message::BufferManager::BufferManager(unsigned int messageSize, unsigned int maxMessageSize,
	unsigned long long channelSize):
	Bufferpool(nullptr),
	MessageBufferSize(messageSize),
	MaxMessageSize(maxMessageSize),
	TotalBufferSize(messageSize * maxMessageSize),
	ChannelSize(channelSize),
	FreeBufferChannel(nullptr)
{
}

void Message::BufferManager::Init()
{
	this->Bufferpool = new char[TotalBufferSize];
	std::cout << "BufferManager::Init() - Allocated " << TotalBufferSize << " bytes for buffer pool." << std::endl;
	std::cout << "Message Size: " << MessageBufferSize << ", Max Message Count: " << MaxMessageSize << std::endl;

	FreeBufferChannel = new std::pair<std::stack<char*>, std::mutex*>[ChannelSize];
	for (int i = 0; i < ChannelSize; ++i)
	{
		this->FreeBufferChannel[i].first = std::stack<char*>();
		this->FreeBufferChannel[i].second = new std::mutex();
	}
	
	for (unsigned int i = 0; i < MaxMessageSize; ++i)
	{
		int ChannelIndex = i % 10;
		char* BufferAddress = this->Bufferpool + (i * MessageBufferSize);
		this->FreeBufferChannel[ChannelIndex].first.push(BufferAddress);
	}
}

Message::BufferManager::~BufferManager()
{
	delete[] Bufferpool;
	for (int i = 0; i < 10; ++i)
	{
		delete FreeBufferChannel[i].second;
	}
	std::cout << "BufferManager::~BufferManager() - Released buffer pool." << std::endl;
}

bool Message::BufferManager::GetMessageBuffer(char*& outBuffer, unsigned int& outBufferSize, int channelIndex)
{
	channelIndex = channelIndex % ChannelSize;
	if (channelIndex < 0)
	{
		return GetBufferCommon(outBuffer, outBufferSize);
	}

	if(!GetBufferByChannel(outBuffer, outBufferSize, channelIndex))
	{
		return GetBufferCommon(outBuffer, outBufferSize);
	}
	return true;
}


bool Message::BufferManager::GetBufferCommon(char*& outBuffer, unsigned int& outBufferSize)
{
	for (int i = 0; i < 10; ++i)
	{
		if(GetBufferByChannel(outBuffer, outBufferSize, i))
		{
			return true;
		}
	}
	return false;
}

bool Message::BufferManager::GetBufferByChannel(char*& outBuffer, unsigned int& outBufferSize, int channelIndex)
{
	if (channelIndex < 0 || channelIndex >= 10)
	{
		return false;
	}

	auto& [stack, mtx] = FreeBufferChannel[channelIndex];
	mtx->lock();
	if (stack.empty())
	{
		mtx->unlock();
		return false;
	}
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

	if (buffer - Bufferpool < 0 || buffer >= Bufferpool + MaxMessageSize * MessageBufferSize)
	{
		return false;
	}

	unsigned long long index = (buffer - Bufferpool) / MessageBufferSize;

	unsigned int channelIndex = index % 10;
	auto& [stack, mtx] = FreeBufferChannel[channelIndex];

	mtx->lock();
	stack.push(buffer);
	mtx->unlock();
	return true;
}

unsigned long long Message::BufferManager::GetAvailableBufferCount()
{
	unsigned long long total = 0;
	for (int i = 0; i < 10; ++i)
	{
		total += FreeBufferChannel[i].first.size();
	}

	return total;
}

void Message::BufferManager::TestBufferwrite()
{
	std::cout << "BufferManager::TestBufferwrite() - Writing test pattern to buffer pool." << std::endl;
	for(unsigned int i = 0; i < TotalBufferSize; ++i)
	{
		Bufferpool[i] = 'A' + (i % 26);
	}
	std::cout << "Is this using Memory Size: " << TotalBufferSize << " bytes?" << std::endl;
}