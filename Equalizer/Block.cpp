#include "Block.h"

std::mutex g_eventLock;
std::condition_variable g_eventReceived;

void Log(const char* str)
{
	std::cout << str << std::endl;
}

Block::Block()
	:m_currentData(nullptr),
	event(EVENT_NO_EVENTS)
{
}

Block::~Block()
{
	if (m_currentData)
	{
		delete(m_currentData);
	}
}

void Block::Run()
{
 	std::unique_lock<std::mutex> locker(g_eventLock);
	
	while (true)
	{
		while (event == EVENT_NO_EVENTS)
		{
 			g_eventReceived.wait(locker);
		}

		HandleEvent();
		event = EVENT_NO_EVENTS;
	}
}

void Block::OnEvent(Events event)
{
	this->event = event;
	g_eventReceived.notify_all();
}