#include "Summator.h"

Summator::Summator(const unsigned short numOfBands)
	:numOfBands(numOfBands),
	sum(nullptr),
	numOfChunks(0),
	sampleDelay(10000),
	delay(false),
	clipping(false),
	sclipping(false),
	standartMaxClippingAmp(1000),
	standartMaxSClippingAmp(100000),
	clippingCoef(10),
	maxClippingAmp(10000),
	maxSClippingAmp(1000000)
{
	prevLastCounts = new short[sampleDelay];

	for (unsigned short i = 0; i < sampleDelay; ++i)
	{
		prevLastCounts[i] = 0;
	}
}

Summator::~Summator()
{
	delete[](prevLastCounts);
}

void Summator::HandleEvent()
{
	switch (event)
	{
	case EVENT_NEW_DATA_RECEIVED:
		if (numOfChunks == 0)
		{
			Reallocate();
		}

		std::unique_lock<std::mutex> locker(dataLock);

		for (unsigned long i = 0; i < defaultChunkSize; ++i)
		{
			sum[i] += currentData[i];
		}

		delete[](currentData);
		currentData = nullptr;
		dataProcessed.notify_one();

		numOfChunks = ++numOfChunks % numOfBands;

		if (numOfChunks == 0)
		{
			const unsigned short startPos = defaultChunkSize - sampleDelay;
			for (unsigned short i = 0; i < sampleDelay; ++i)
			{
				prevLastCounts[i] = sum[startPos + i];
			}

			if (delay)
			{
				ApplyDelay();
			}

			if (clipping)
			{
				ApplyClipping();
			}

			if (sclipping)
			{
				ApplySClipping();
			}

			if (output)
			{
				dynamic_cast<DataHandler*>(output)->SendNewData(sum);
			}
		}

		break;
	}
}

void Summator::Reallocate()
{
	sum = new short[defaultChunkSize];

	for (unsigned long i = 0; i < defaultChunkSize; ++i)
	{
		sum[i] = 0;
	}
}

void Summator::SetEffect(Effects effect, bool reset)
{
	switch (effect)
	{
	case EFFECT_DELAY:
		delay = reset ? false : !delay;
		break;
	case EFFECT_CLIPPING:
		clipping = reset ? false : !clipping;
		break;
	case EFFECT_SCLIPPING:
		sclipping = reset ? false : !sclipping;
	}
}

void Summator::ApplyDelay()
{
	short* y = new short[defaultChunkSize];

	for (long i = 0; i < defaultChunkSize; ++i)
	{
		y[i] = sum[i] + (i - (long)sampleDelay >= 0 ? sum[i - sampleDelay] : prevLastCounts[i]);
	}

	delete(sum);
	sum = y;
}

void Summator::ApplyClipping()
{
	for (unsigned long i = 0; i < defaultChunkSize; ++i)
	{
		if (sum[i] > maxClippingAmp)
		{
			sum[i] = maxClippingAmp;
		}
		else if (sum[i] < -maxClippingAmp)
		{
			sum[i] = -maxClippingAmp;
		}
	}
}

void Summator::SetClippingCoef(const short coef)
{
	clippingCoef = coef;
	maxClippingAmp = (short)(standartMaxClippingAmp * clippingCoef);
	maxSClippingAmp = (long)(standartMaxSClippingAmp * clippingCoef);
}

void Summator::ApplySClipping()
{
	const unsigned long memSize = defaultChunkSize * sizeof(fftw_complex);
	fftw_complex* complexBuffer = (fftw_complex*)fftw_malloc(memSize);

	for (unsigned long i = 0; i < defaultChunkSize; ++i)
	{
		complexBuffer[i][0] = (double)(sum[i]);
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

		if (A > maxSClippingAmp)
		{
			A = maxSClippingAmp;
		}

		complexBuffer[i][0] = A * cos(phi);
		complexBuffer[i][1] = A * sin(phi);
	}

	fftw_plan invPlan = fftw_plan_dft_1d(defaultChunkSize, complexBuffer, complexBuffer, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(invPlan);

	for (unsigned long i = 0; i < defaultChunkSize; ++i)
	{
		sum[i] = (short)(complexBuffer[i][0] / defaultChunkSize);
	}

	fftw_free(complexBuffer);

	fftw_destroy_plan(plan);
	fftw_destroy_plan(invPlan);
}