#pragma once

#include "Filter.h"
#include "icoefs.h"

class IIR : public Filter
{
public:
	IIR(const unsigned short num, const unsigned short numOfBands);
	~IIR();

	void SetGain(const double mult);

private:
	const short* Filtering();

	short* m_prevOutLastCounts;
};