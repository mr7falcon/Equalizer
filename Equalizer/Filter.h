 #pragma once
 
#include "DataHandler.h"
#include "fftw3.h"
#include "fcoefs.h"
  
 class Filter : public DataHandler
 {
 public:
 	Filter(const unsigned short num, const unsigned short numOfBands);
	~Filter();
 
	void SetGain(double mult);
	double GetGain() const;
 
 protected:
	const unsigned short num;
	const unsigned short numOfBands;
	static unsigned short numProcessed;
	unsigned short order;
 
 	//short int* Transform(const unsigned long size, short int* counts, bool backward = false) const;
 	virtual const short* Filtering() = 0;

	void HandleEvent();

	short* m_prevLastCounts;

	double m_gain;
 };