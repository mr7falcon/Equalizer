#include "Summator.h"

Summator::Summator(const unsigned short numOfBands)
	:numOfBands(numOfBands),
	m_numOfChunks(0),
	m_sampleDelay(10000)
{
	m_prevLastCounts = new short[m_sampleDelay];

	for (unsigned short i = 0; i < m_sampleDelay; ++i)
	{
		m_prevLastCounts[i] = 0;
	}
}

Summator::~Summator()
{
	delete[](m_prevLastCounts);
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
			const unsigned short startPos = defaultChunkSize - m_sampleDelay;
			for (unsigned short i = 0; i < m_sampleDelay; ++i)
			{
				m_prevLastCounts[i] = m_summ[startPos + i];
			}

			if (m_delay)
			{
				ApplyDelay();
			}

			if (m_clipping)
			{
				ApplyClipping();
			}

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

void Summator::SetEffect(Effects effect, bool reset)
{
	switch (effect)
	{
	case EFFECT_DELAY:
		m_delay = reset ? false : !m_delay;
		break;
	case EFFECT_CLIPPING:
		m_clipping = reset ? false : !m_clipping;
		break;
	}
}

void Summator::ApplyDelay()
{
	short* y = new short[defaultChunkSize];

	for (long i = 0; i < defaultChunkSize; ++i)
	{
		y[i] = m_summ[i] + (i - (long)m_sampleDelay >= 0 ? m_summ[i - m_sampleDelay] : m_prevLastCounts[i]);
	}

	delete(m_summ);
	m_summ = y;
}

void Summator::ApplyClipping()
{

}