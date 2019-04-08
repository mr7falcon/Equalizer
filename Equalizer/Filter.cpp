#include "Filter.h"

Filter::Filter(const unsigned short num, const unsigned short numOfBands, const unsigned short order, const double coef)
	:num(num),
	order(order),
	m_gain(0),
	coef(coef),
	m_mult(coef),
	numOfBands(numOfBands)
{
}

Filter::~Filter()
{
}

unsigned short Filter::numProcessed;

void Filter::HandleEvent()
{
	switch (event)
	{
	case EVENT_NEW_DATA_RECEIVED:
		std::unique_lock<std::mutex> locker(g_dataLock);

		const double* filteredCounts = Filtering();

		short* dataToSend = new short[defaultChunkSize];

		for (unsigned long i = 0; i < defaultChunkSize; ++i)
		{
			dataToSend[i] = (short)(m_mult * filteredCounts[i]);
		}

		delete[](filteredCounts);

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
			dynamic_cast<DataHandler*>(output)->SendNewData(dataToSend);
		}

		break;
	}
}

void Filter::SetGain(const double mult)
{
	m_gain = mult;
	m_mult = coef * std::pow(10, m_gain / 20);
}

double Filter::GetGain() const
{
	return m_gain;
}