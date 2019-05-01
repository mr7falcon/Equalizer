 #include "FIR.h"
 
FIR::FIR(const unsigned short num, const unsigned short numOfBands)
	:Filter(num, numOfBands, FCL - 1, 0.15)
{
	prevLastCounts = new short[order + 1];

	for (unsigned short i = 0; i <= order; ++i)
	{
		prevLastCounts[i] = 0;
	}
}

FIR::~FIR()
{
	delete[](prevLastCounts);
}

const double* FIR::Filtering()
{
	double* filtredCounts = nullptr;

	if (currentData)
	{
		filtredCounts = new double[defaultChunkSize];

		for (long i = 0; i < defaultChunkSize; ++i)
		{
			filtredCounts[i] = 0;

			for (short j = 0; j <= order; ++j)
			{
				if (i - j >= 0)
				{
					filtredCounts[i] += B[num][j] * currentData[i - j];
				}
				else
				{
					filtredCounts[i] += B[num][j] * prevLastCounts[order + (i - j - 1)];
				}
			}
		}

		const unsigned long lastCountsStart = defaultChunkSize - order - 1;
		for (unsigned short i = 0; i <= order; ++i)
		{
			prevLastCounts[i] = currentData[lastCountsStart + i];
		}
	}

	return filtredCounts;
}