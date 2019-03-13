#pragma once

#include <iostream>
#include <thread>
#include <initguid.h>
#include <dsound.h>
#include <mutex>
#include <windows.h>
#include <process.h>
#include "WaveFormat.h"

extern std::recursive_mutex g_lock;

void Log(const char* str);

enum Events : short int
{
	EVENT_FIRST_HALF_BUFFER_PLAYED,
	EVENT_SECOND_HALF_BUFFER_PLAYED,
	EVENT_NEW_DATA_RECEIVED,
	EVENT_NEW_DATA_REQUESTED,
	
	EVENT_NO_EVENTS = -1
};

class Block
{
public:
	Block();
	~Block();

	void OnEvent(Events event) { this->event = event; }
	void SetOutput(Block* output) { this->output = output; }
	virtual void Run();

protected:
	virtual void HandleEvent() = 0;

	Block* output;

	DataChunk* m_currentData;

	Events event;
};
