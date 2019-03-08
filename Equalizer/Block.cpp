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
	m_newData(nullptr)
{
}

Block::~Block()
{
}

void Block::SendNewData(DataChunk* newCurrentData)
{
	m_newData = newCurrentData;
}

void Block::Run()
{
	while (true)
	{
		if (m_newData)
		{
			g_lock.lock();

			m_currentData = m_newData;
			m_newData = nullptr;
			HandleData();

			g_lock.unlock();
		}
	}
}