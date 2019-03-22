 #pragma once
 
 #include "Filter.h"
 
 class FIR : public Filter
 {
 public:
	FIR(const unsigned short num);

 private:
 	short* Filtering();
 };