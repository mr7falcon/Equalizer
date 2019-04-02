 #pragma once
 
#include "DataHandler.h"
  
 class Filter : public DataHandler
 {
 public:
 	Filter(const unsigned short num, const unsigned short numOfBands, const unsigned short order, const double mult);
	~Filter();
 
	virtual void SetGain(const double mult);
	double GetGain() const;
 
 protected:
	const unsigned short num;
	const unsigned short numOfBands;
	static unsigned short numProcessed;
	unsigned short order;
 
 	virtual const short* Filtering() = 0;

	void HandleEvent();

	short* m_prevLastCounts;

	double m_gain;
	double m_mult;
 };