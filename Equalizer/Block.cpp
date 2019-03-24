#include "Block.h"

void Log(const char* str)
{
	std::mutex m;
	m.lock();
	std::cout << str << std::endl;
	m.unlock();
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
		delete[](m_currentData);
	}
}

void Block::Run()
{
	while (true)
	{
 		std::unique_lock<std::mutex> locker(g_eventLock);
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
	std::unique_lock<std::mutex> locker(g_eventLock);

	this->event = event;

	g_eventReceived.notify_one();
}