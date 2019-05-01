#include "Filter.h"

Filter::Filter(const unsigned short num, const unsigned short numOfBands, const unsigned short order, const double coef)
	:num(num),
	order(order),
	gain(0),
	coef(coef),
	mult(1),
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
		std::unique_lock<std::mutex> locker(dataLock);

		const double* filteredCounts = Filtering();

		short* dataToSend = new short[defaultChunkSize];

		for (unsigned long i = 0; i < defaultChunkSize; ++i)
		{
			dataToSend[i] = (short)(coef * filteredCounts[i]);
			dataToSend[i] *= mult;
		}

		delete[](filteredCounts);

		++numProcessed;
		if (numProcessed == numOfBands)
		{
			delete[](currentData);
			numProcessed = 0;
		}

		currentData = nullptr;
		dataProcessed.notify_one();

		if (output)
		{
			dynamic_cast<DataHandler*>(output)->SendNewData(dataToSend);
		}

		break;
	}
}

void Filter::SetGain(const double gain)
{
	this->gain = gain;
	mult = std::pow(10, this->gain / 20);
}

double Filter::GetGain() const
{
	return gain;
}