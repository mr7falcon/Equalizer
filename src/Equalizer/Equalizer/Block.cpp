#include "Block.h"

std::mutex g_logLock;

void Log(const char* str)
{
	g_logLock.lock();
	std::cout << str << std::endl;
	g_logLock.unlock();
}

Block::Block()
	:currentData(nullptr),
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
 		std::unique_lock<std::mutex> locker(eventLock);
		while (event == EVENT_NO_EVENTS)
		{
 			eventReceived.wait(locker);
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
	std::unique_lock<std::mutex> locker(eventLock);

	this->event = event;

	eventReceived.notify_one();
}