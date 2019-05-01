 #pragma once
 
#include "DataHandler.h"
  
 class Filter : public DataHandler
 {
 public:
 	Filter(const unsigned short num, const unsigned short numOfBands, const unsigned short order, const double coef);
	~Filter();
 
	void SetGain(const double gain);
	double GetGain() const;
 
 protected:
	const unsigned short num;
	const unsigned short numOfBands;
	static unsigned short numProcessed;
	
	const unsigned short order;
 
 	virtual const double* Filtering() = 0;

	void HandleEvent();

	double gain;
	double mult;
	const double coef;
 };