#include "Filter.h"

Filter::Filter(const unsigned short num, const unsigned short numOfBands)
	:num(num),
	order(BL - 1),
	m_gain(1),
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

void Filter::Init(unsigned short bitsPerSample)
{
	this->bitsPerSample = bitsPerSample;
}

//short int* Filter::Transform(const unsigned long size, short int* counts, bool backward) const
//{
//	short int* transformedCounts = nullptr;
//
//	if (counts)
//	{
//		transformedCounts = new short int[size];
//		fftw_plan plan = fftw_plan_dft_1d(size, (fftw_complex*)(&counts), (fftw_complex*)(&transformedCounts), backward ? FFTW_BACKWARD : FFTW_FORWARD, FFTW_ESTIMATE);
//		fftw_execute(plan);
//		fftw_destroy_plan(plan);
//	}
//
//	return transformedCounts;
//}

void Filter::HandleEvent()
{
	switch (event)
	{
	case EVENT_NEW_DATA_RECEIVED:
		std::unique_lock<std::mutex> locker(g_dataLock);

		const short* filteredCounts = Filtering();

		//ƒа, костыль, но по другому пока не придумал
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

void Filter::SetGain(float mult)
{
	m_gain = mult < 0.f ? 1 / mult : mult;
}

float Filter::GetGain() const
{
	return m_gain;
}