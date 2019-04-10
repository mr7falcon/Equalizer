 #include "IIR.h"
 
 IIR::IIR(const unsigned short num, const unsigned short numOfBands)
	 :Filter(num, numOfBands, ICL - 1, 0.005)
 {
 }

 IIR::~IIR()
 {
 }

 const double* IIR::Filtering()
 {
	 double* filtredCounts = nullptr;

	 if (m_currentData)
	 {
		 double w[3];

		 filtredCounts = new double[defaultChunkSize];

		 for (unsigned long i = 0; i < defaultChunkSize; ++i)
		 {
			 w[1] = 0;
			 w[2] = 0;

			 filtredCounts[i] = m_currentData[i];

			 for (unsigned short j = 1; j <= order; j += 2)
			 {
				 filtredCounts[i] *= NUM[num][j - 1][0];
				 w[0] = filtredCounts[i] - DEN[num][j][1] * w[1] - DEN[num][j][2] * w[2];
				 filtredCounts[i] = NUM[num][j][0] * w[0] + NUM[num][j][1] * w[1] + NUM[num][j][2] * w[2];

				 w[2] = w[1];
				 w[1] = w[0];
			 }

			 filtredCounts[i] *= NUM[num][order][0];
		 }
	 }

	 return filtredCounts;
 }