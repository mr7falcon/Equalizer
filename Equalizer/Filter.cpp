#include "Filter.h"

Filter::Filter(const unsigned short num, const unsigned short numOfBands, const unsigned short order, const double mult)
	:num(num),
	order(order),
	m_gain(0),
	m_mult(mult),
	numOfBands(numOfBands)
{
	m_prevLastCounts = new short[order + 1];

	for (unsigned short i = 0; i <= order; ++i)
	{
		m_prevLastCounts[i] = 0;
	}
}

Filter::~Filter()
{
	delete[](m_prevLastCounts);
}

unsigned short Filter::numProcessed;

void Filter::HandleEvent()
{
	switch (event)
	{
	case EVENT_NEW_DATA_RECEIVED:
		std::unique_lock<std::mutex> locker(g_dataLock);

		const short* filteredCounts = Filtering();

		//ƒа, костыль, но по другому пока не придумал. — shared_ptr вообще кака€-то чушн€ происходит
		++numProcessed;
		if (numProcessed == numOfBands)
		{
			delete[](m_currentData);
			numProcessed = 0;
		}

		m_currentData = nullptr;
		g_dataProcessed.notify_one();

		if (output)
		{
			dynamic_cast<DataHandler*>(output)->SendNewData(filteredCounts);
		}

		break;
	}
}

void Filter::SetGain(const double mult)
{
	m_gain = mult;
}

double Filter::GetGain() const
{
	return m_gain;
}