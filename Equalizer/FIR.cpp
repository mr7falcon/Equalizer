 #include "FIR.h"
 
FIR::FIR(const unsigned short num)
	:Filter(num)
{
}

short* FIR::Filtering()
{
	short int* filtredCounts = nullptr;

	if (m_currentData)
	{
		filtredCounts = new short int[m_currentData->size];

		for (unsigned long i = 0; i < m_currentData->size; ++i)
		{
			filtredCounts[i] = 0;
		}

		for (long i = 0; i < m_currentData->size; ++i)
		{
			for (short j = 0; j <= order; ++j)
			{
				if (i - j >= 0)
				{
					filtredCounts[i] += (short)(B[num][j] * m_currentData->data[i - j] * m_gain);
				}
				else
				{
					filtredCounts[i] += (short)(B[num][j] * m_prevLastCounts[order + (i - j - 1)] * m_gain);
				}
			}
		}

		const unsigned long lastCountsStart = m_currentData->size - order - 1;
		for (unsigned long i = 0; i <= order; ++i)
		{
			m_prevLastCounts[i] = m_currentData->data[lastCountsStart + i];
		}
	}

	return filtredCounts;
}