#pragma once

#include <stack>
#include <mutex>
#include <semaphore>

namespace Message
{
	class BufferManager
	{
	private:
		bool bInit = false;

		char* Bufferpool = nullptr;
		unsigned int MessageSize = 192;
		unsigned int MaxMessageSize = 10'000'000;
		unsigned int TotalBufferSize = 0;

		std::pair<std::stack<char*>, std::mutex*> FreeBufferChannel[10];

		bool GetBufferCommon(char*& outBuffer, int& outBufferSize);
		bool GetBufferByChannel(int channelIndex, char*& outBuffer, int& outBufferSize);

	public:
		BufferManager();
		~BufferManager();
		void Init(unsigned int MessageSize = 192, unsigned int MaxMessageSize = 10'000'000); // Default 2GB
		bool GetMessageBuffer(char*& outBuffer, int& outBufferSize, int channelIndex = -1);
		bool ReleaseMessageBuffer(char* buffer);
	};
}