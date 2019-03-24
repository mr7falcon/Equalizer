#pragma once

#include <iostream>
#include <thread>
#include <initguid.h>
#include <dsound.h>
#include <windows.h>
#include <process.h>
#include <mutex>
#include <condition_variable>
#include "WaveFormat.h"

void Log(const char* str);

enum Events : short int
{
	EVENT_NEW_DATA_RECEIVED,
	EVENT_NEW_DATA_REQUESTED,
	EVENT_SECTION_PLAYED,
	
	EVENT_NO_EVENTS = -1
};

class Block
{
public:
	Block();
	virtual ~Block();

	void OnEvent(Events event);
	virtual void SetOutput(Block* output) = 0;
	virtual void Run();

protected:
	virtual void HandleEvent() = 0;

	const short* m_currentData;

	Events event;

	std::mutex g_eventLock;
	std::condition_variable g_eventReceived;
};
