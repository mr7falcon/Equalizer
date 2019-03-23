#include "Filter.h"

Filter::Filter(const unsigned short num)
	:num(num),
	order(BL - 1),
	m_gain(1)
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

		short* filteredCounts = Filtering();
		DataChunk* filteredData = new DataChunk(m_currentData->size, filteredCounts);

		m_currentData = nullptr;
		g_dataProcessed.notify_all();

		if (output)
		{
			dynamic_cast<DataHandler*>(output)->SendNewData(filteredData);
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