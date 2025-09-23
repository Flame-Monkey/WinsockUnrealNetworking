#include "BufferManager.h"
#include <iostream>
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
	// 동시적으로 MessageManager를 생성하면, ReferenceCount 동시성 문제가 있음. Release에서도 마찬가지
	// 어차피 메시지매니저의 각 메소드나, Init, Destroyer는 한 스레드씩에서 호출돼 동시성 문제 없을거같긴한데
	ReferenceCount++;
	if (bInit)
	{
		return;
	}
	this->MessageSize = MessageSize;
	this->MaxMessageSize = MaxMessageSize;
	this->TotalBufferSize = MessageSize * MaxMessageSize;
	this->Bufferpool = new char[TotalBufferSize];
	std::cout << "BufferManager::Init() - Allocated " << TotalBufferSize << " bytes for buffer pool." << std::endl;
	std::cout << "Message Size: " << MessageSize << ", Max Message Count: " << MaxMessageSize << std::endl;

	for (int i = 0; i < 10; ++i)
	{
		this->FreeBufferChannel[i].first = std::stack<char*>();
		this->FreeBufferChannel[i].second = new std::mutex();
	}
	
	for (unsigned int i = 0; i < MaxMessageSize; ++i)
	{
		int ChannelIndex = i % 10;
		char* BufferAddress = this->Bufferpool + (i * MessageSize);
		this->FreeBufferChannel[ChannelIndex].first.push(BufferAddress);
	}

	bInit = true;
}

void Message::BufferManager::ReleaseBufferManager()
{
	ReferenceCount--;
	if (ReferenceCount <= 0)
	{
		ReferenceCount = 0;
		bInit = false;
		delete[] Bufferpool;
		Bufferpool = nullptr;
		for (int i = 0; i < 10; ++i)
		{
			while (!FreeBufferChannel[i].first.empty())
			{
				FreeBufferChannel[i].first.pop();
			}
			delete FreeBufferChannel[i].second;
			FreeBufferChannel[i].second = nullptr;
		}
	}
}

Message::BufferManager::~BufferManager()
{
	delete[] Bufferpool;
	for (int i = 0; i < 10; ++i)
	{
		delete FreeBufferChannel[i].second;
	}
}

bool Message::BufferManager::GetMessageBuffer(char*& outBuffer, unsigned int& outBufferSize, int channelIndex)
{
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

	if (buffer - Bufferpool < 0 || buffer >= Bufferpool + MaxMessageSize * MessageSize)
	{
		return false;
	}

	unsigned long long index = (buffer - Bufferpool) / MessageSize;

	unsigned int channelIndex = index % 10;
	auto& [stack, mtx] = FreeBufferChannel[channelIndex];

	mtx->lock();
	stack.push(buffer);
	mtx->unlock();
	return true;
}