#pragma once

#include <iostream>
#include <thread>
#include <dsound.h>
#include <mutex>
#include "WaveFormat.h"

extern std::mutex g_lock;

void Log(const char* str);

class Block
{
public:
	Block(Block* output);
	~Block();

	void OnInput(DataChunk* data);

protected:
	virtual void HandleDataInternal(DataChunk* data) = 0;
	void SendData(DataChunk* data) const;

	Block* output;
};

void HandleData(Block* block, DataChunk* data);
