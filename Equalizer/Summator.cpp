#include "Summator.h"

Summator::Summator(const unsigned short numOfBands)
	:numOfBands(numOfBands),
	m_summ(nullptr),
	m_numOfChunks(0),
	m_sampleDelay(10000),
	m_delay(false),
	m_clipping(false),
	m_sclipping(false),
	m_standartMaxClippingAmp(1000),
	m_standartMaxSClippingAmp(100000),
	m_clippingCoef(10),
	m_maxClippingAmp(10000),
	m_maxSClippingAmp(1000000)
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

			if (m_sclipping)
			{
				ApplySClipping();
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
	case EFFECT_SCLIPPING:
		m_sclipping = reset ? false : !m_sclipping;
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
	for (unsigned long i = 0; i < defaultChunkSize; ++i)
	{
		if (m_summ[i] > m_maxClippingAmp)
		{
			m_summ[i] = m_maxClippingAmp;
		}
		else if (m_summ[i] < -m_maxClippingAmp)
		{
			m_summ[i] = -m_maxClippingAmp;
		}
	}
}

void Summator::SetClippingCoef(const short coef)
{
	m_clippingCoef = coef;
	m_maxClippingAmp = (short)(m_standartMaxClippingAmp * m_clippingCoef);
	m_maxSClippingAmp = (long)(m_standartMaxSClippingAmp * m_clippingCoef);
}

void Summator::ApplySClipping()
{
	const unsigned long memSize = defaultChunkSize * sizeof(fftw_complex);
	fftw_complex* complexBuffer = (fftw_complex*)fftw_malloc(memSize);

	for (unsigned long i = 0; i < defaultChunkSize; ++i)
	{
		complexBuffer[i][0] = (double)(m_summ[i]);
		complexBuffer[i][1] = 0.;
	}

	fftw_plan plan = fftw_plan_dft_1d(defaultChunkSize, complexBuffer, complexBuffer, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(plan);

	double A = 0;
	double phi = 0;

	for (unsigned long i = 0; i < defaultChunkSize; ++i)
	{
		if (complexBuffer[i][0] == 0)
		{
			phi = complexBuffer[i][1] > 0 ? pi / 2 : -pi / 2;
		}
		else if (complexBuffer[i][0] > 0)
		{
			phi = atan(complexBuffer[i][1] / complexBuffer[i][0]);
		}
		else
		{
			phi = atan(complexBuffer[i][1] / complexBuffer[i][0]) + (complexBuffer[i][1] >= 0 ? pi : -pi);
		}

		A = sqrt(complexBuffer[i][0] * complexBuffer[i][0] + complexBuffer[i][1] * complexBuffer[i][1]);

		if (A > m_maxSClippingAmp)
		{
			A = m_maxSClippingAmp;
		}

		complexBuffer[i][0] = A * cos(phi);
		complexBuffer[i][1] = A * sin(phi);
	}

	fftw_plan invPlan = fftw_plan_dft_1d(defaultChunkSize, complexBuffer, complexBuffer, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(invPlan);

	for (unsigned long i = 0; i < defaultChunkSize; ++i)
	{
		m_summ[i] = (short)(complexBuffer[i][0] / defaultChunkSize);
	}

	fftw_free(complexBuffer);

	fftw_destroy_plan(plan);
	fftw_destroy_plan(invPlan);
}