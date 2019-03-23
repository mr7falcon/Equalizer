#include "Summator.h"

Summator::Summator(const unsigned short numOfBands)
	:numOfBands(numOfBands),
	m_numOfChunks(0)
{
}

Summator::~Summator()
{
	if (m_summ)
	{
		delete[](m_summ);
	}
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

		for (unsigned long i = 0; i < m_currentData->size; ++i)
		{
			m_summ->data[i] += m_currentData->data[i];
		}

		delete(m_currentData);
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
	m_summ = new DataChunk(defaultChunkSize);

	for (unsigned long i = 0; i < m_summ->size; ++i)
	{
		m_summ->data[i] = 0;
	}
}