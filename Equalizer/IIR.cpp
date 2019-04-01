 #include "IIR.h"
 
 IIR::IIR(const unsigned short num, const unsigned short numOfBands)
	 :Filter(num, numOfBands, ICL - 1)
 {
 }

 const short* IIR::Filtering()
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
					 for (int z = 0; z < NL[j]; ++z)
					 {
						 filtredCounts[i] += (short)(NUM[num][j][z] * m_currentData[i - j]);
					 }
				 }
				 else
				 {
					 for (int z = 0; z < DL[j]; ++z)
					 {
						 filtredCounts[i] += (short)(DEN[num][j][z] * m_prevLastCounts[order + (i - j - 1)]);
					 }
				 }
			 }

			 filtredCounts[i] *= m_gain;
		 }

		 const unsigned long lastCountsStart = defaultChunkSize - order - 1;
		 for (unsigned long i = 0; i <= order; ++i)
		 {
			 m_prevLastCounts[i] = m_currentData[lastCountsStart + i];
		 }
	 }

	 return filtredCounts;
 }