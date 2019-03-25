#include "Summator.h"

Summator::Summator(const unsigned short numOfBands)
	:numOfBands(numOfBands),
	m_numOfChunks(0)
{
}

Summator::~Summator()
{
}

void Summator::HandleEvent()
{
	switch (event)
	{
	case EVENT_NEW_DATA_RECEIVED:
		if (m_numOfChunks == 0)
		{
			Reallocate();
		}

		std::unique_lock<std::mutex> locker(g_dataLock);

		for (unsigned long i = 0; i < defaultChunkSize; ++i)
		{
			m_summ[i] += m_currentData[i];
		}

		delete[](m_currentData);
		m_currentData = nullptr;
		g_dataProcessed.notify_one();

		m_numOfChunks = ++m_numOfChunks % numOfBands;

		if (m_numOfChunks == 0)
		{
			if (output)
			{
				dynamic_cast<DataHandler*>(output)->SendNewData(m_summ);
			}
		}

		break;
	}
}

void Summator::Reallocate()
{
	m_summ = new short[defaultChunkSize];

	for (unsigned long i = 0; i < defaultChunkSize; ++i)
	{
		m_summ[i] = 0;
	}
}