 #include "FIR.h"
 
FIR::FIR(const unsigned short num, const unsigned short numOfBands)
	:Filter(num, numOfBands)
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

		const float mult = m_gain == 1.f ? 0.145f : 0.13 * m_gain;

		for (long i = 0; i < defaultChunkSize; ++i)
		{
			for (short j = 0; j <= order; ++j)
			{
				if (i - j >= 0)
				{
					filtredCounts[i] += (short)(B[num][j] * m_currentData[i - j] * mult);
				}
				else
				{
					filtredCounts[i] += (short)(B[num][j] * m_prevLastCounts[order + (i - j - 1)] * mult);
				}
			}
		}

		const unsigned long lastCountsStart = defaultChunkSize - order - 1;
		for (unsigned long i = 0; i <= order; ++i)
		{
			m_prevLastCounts[i] = m_currentData[lastCountsStart + i];
		}
	}

	return filtredCounts;
}