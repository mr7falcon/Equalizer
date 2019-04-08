 #include "IIR.h"
 
 IIR::IIR(const unsigned short num, const unsigned short numOfBands)
	 :Filter(num, numOfBands, ICL - 1, 0.01)
 {
 }

 IIR::~IIR()
 {
 }

 const short* IIR::Filtering()
 {
	 short* filtredCounts = nullptr;

	 if (m_currentData)
	 {
		 double w[3];

		 double temp[defaultChunkSize];

		 for (unsigned long i = 0; i < defaultChunkSize; ++i)
		 {
			 w[1] = 0;
			 w[2] = 0;

			 temp[i] = m_currentData[i];

			 for (unsigned short j = 1; j <= order; j += 2)
			 {
				 temp[i] *= NUM[num][j - 1][0];
				 w[0] = temp[i] - DEN[num][j][1] * w[1] - DEN[num][j][2] * w[2];
				 temp[i] = NUM[num][j][0] * w[0] + NUM[num][j][1] * w[1] + NUM[num][j][2] * w[2];

				 w[2] = w[1];
				 w[1] = w[0];
			 }

			 temp[i] *= NUM[num][order][0];
		 }

		 filtredCounts = new short[defaultChunkSize];

		 for (unsigned long i = 0; i < defaultChunkSize; ++i)
		 {
			 filtredCounts[i] = (short)(m_mult * temp[i]);
		 }
	 }

	 return filtredCounts;
 }