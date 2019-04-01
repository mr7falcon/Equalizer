 #pragma once
 
#include "DataHandler.h"
  
 class Filter : public DataHandler
 {
 public:
 	Filter(const unsigned short num, const unsigned short numOfBands, const unsigned short order);
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