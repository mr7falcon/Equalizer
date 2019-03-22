#include "Block.h"

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
	while (true)
	{
		if (event != EVENT_NO_EVENTS)
		{
			HandleEvent();
			event = EVENT_NO_EVENTS;
		}
	}
}