#include "Block.h"

std::mutex g_logLock;

void Log(const char* str)
{
	g_logLock.lock();
	std::cout << str << std::endl;
	g_logLock.unlock();
}

Block::Block()
	:m_currentData(nullptr),
	event(EVENT_NO_EVENTS)
{
}

Block::~Block()
{
}

void Block::Run()
{
	while (true)
	{
 		std::unique_lock<std::mutex> locker(m_eventLock);
		while (event == EVENT_NO_EVENTS)
		{
 			m_eventReceived.wait(locker);
		}

		if (event == EVENT_SHUTDOWN)
		{
			return;
		}
		else
		{
			HandleEvent();
		}

		event = EVENT_NO_EVENTS;
	}
}

void Block::OnEvent(Events event)
{
	std::unique_lock<std::mutex> locker(m_eventLock);

	this->event = event;

	m_eventReceived.notify_one();
}