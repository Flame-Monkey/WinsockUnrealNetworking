#pragma once

#include <stack>
#include <mutex>

namespace Message
{
	class BufferManager
	{
	private:
		char* Bufferpool = nullptr;
		const unsigned int MessageBufferSize;
		const unsigned int MaxMessageSize;
		const unsigned int TotalBufferSize;

		unsigned long long ChannelSize;

		std::pair<std::stack<char*>, std::mutex*> *FreeBufferChannel;

		bool GetBufferCommon(char*& outBuffer, unsigned int& outBufferSize);
		bool GetBufferByChannel(char*& outBuffer, unsigned int& outBufferSize, int channelIndex);

	public:
		BufferManager(unsigned int messageSize = 192, unsigned int maxMessageSize = 10'000'000, 
			unsigned long long channelSize = 10);
		~BufferManager();
		void Init(); // Default 2GB
		bool GetMessageBuffer(char*& outBuffer, unsigned int& outBufferSize, int channelIndex = -1);
		bool ReleaseMessageBuffer(char* buffer);

		unsigned long long GetAvailableBufferCount();

		void TestBufferwrite();
		void PrintStatus();
	};
}