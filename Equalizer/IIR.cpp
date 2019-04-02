 #include "IIR.h"
 
 IIR::IIR(const unsigned short num, const unsigned short numOfBands)
	 :Filter(num, numOfBands, ICL - 1, 0.05)
 {
	 m_prevOutLastCounts = new short[order + 1];

	 for (unsigned short i = 0; i <= order; ++i)
	 {
		 m_prevOutLastCounts[i] = 0;
	 }
 }

 IIR::~IIR()
 {
	 delete[](m_prevOutLastCounts);
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
			 for (short n = 0; n <= order; ++n)
			 {
				 for (short j = 0; j < DL[n]; ++j)
				 {
					 filtredCounts[i] -= (short)(DEN[num][n][j] * (i - j >= 0 ? filtredCounts[i - j] : m_prevOutLastCounts[order + (i - j - 1)]));
				 }

				 for (short j = 0; j < NL[n]; ++j)
				 {
					 filtredCounts[i] += (short)(NUM[num][n][j] * (i - j >= 0 ? m_currentData[i - j] : m_prevLastCounts[order + (i - j - 1)]));
				 }
			 }
		 }

		 const unsigned long lastCountsStart = defaultChunkSize - order - 1;
		 for (unsigned long i = 0; i <= order; ++i)
		 {
			 m_prevLastCounts[i] = m_currentData[lastCountsStart + i];
			 m_prevOutLastCounts[i] = filtredCounts[lastCountsStart + i];
		 }

		 for (unsigned long i = 0; i < defaultChunkSize; ++i)
		 {
			 filtredCounts[i] *= m_mult;
		 }
	 }

	 return filtredCounts;
 }