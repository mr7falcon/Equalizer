 #pragma once
 
#include "Filter.h"
#include "fcoefs.h"
 
 class FIR : public Filter
 {
 public:
	FIR(const unsigned short num, const unsigned short numOfBands);

 private:
 	const short* Filtering();
 };