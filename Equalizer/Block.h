#pragma once

#include <iostream>
#include <thread>
#include <dsound.h>
#include <mutex>
#include <windows.h>
#include <process.h>
#include "WaveFormat.h"

extern std::recursive_mutex g_lock;

void Log(const char* str);

class Block
{
public:
	Block();
	~Block();

	void SendNewData(DataChunk* newCurrentData);
	void SetOutput(Block* output) { this->output = output; }
	void Run();

protected:
	virtual void HandleData() = 0;

	Block* output;

	DataChunk* m_newData;
	DataChunk* m_currentData;
};
