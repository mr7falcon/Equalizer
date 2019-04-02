#pragma once

#include "Filter.h"
#include "icoefs.h"

class IIR : public Filter
{
public:
	IIR(const unsigned short num, const unsigned short numOfBands);
	~IIR() {}
private:
	const short* Filtering();

	short* m_prevOutLastCounts;
};