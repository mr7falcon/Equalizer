 #include "FIR.h"
 
FIR::FIR(const unsigned short num, const unsigned short numOfBands)
	:Filter(num, numOfBands, FCL - 1, 0.145)
{
}

const short* FIR::Filtering()
{
	short* filtredCounts = nullptr;

	if (m_currentData)
	{
		filtredCounts = new short[defaultChunkSize];

		for (unsigned long i = 0; i < defaultChunkSize; ++i)
		{
			filtredCounts[i] = 0;
		}

		for (long i = 0; i < defaultChunkSize; ++i)
		{
			for (short j = 0; j <= order; ++j)
			{
				if (i - j >= 0)
				{
					filtredCounts[i] += (short)(B[num][j] * m_currentData[i - j]);
				}
				else
				{
					filtredCounts[i] += (short)(B[num][j] * m_prevLastCounts[order + (i - j - 1)]);
				}
			}

			filtredCounts[i] *= m_mult;
		}

		const unsigned long lastCountsStart = defaultChunkSize - order - 1;
		for (unsigned long i = 0; i <= order; ++i)
		{
			m_prevLastCounts[i] = m_currentData[lastCountsStart + i];
		}
	}

	return filtredCounts;
}