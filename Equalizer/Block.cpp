#include "Block.h"

std::recursive_mutex g_lock;

void Log(const char* str)
{
	g_lock.lock();

	std::cout << str << std::endl;

	g_lock.unlock();
}

Block::Block()
	:output(nullptr),
	m_currentData(nullptr),
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
		if (event != EVENT_NO_EVENTS)
		{
			HandleEvent();
			event = EVENT_NO_EVENTS;
		}
	}
}