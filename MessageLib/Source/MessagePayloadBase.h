#pragma once

struct MessagePayloadBase
{
public:
	~MessagePayloadBase();
	unsigned int GetLength() ;
	void Serialize(char* data);
	void Deserialize(const char* data, unsigned int size);
};