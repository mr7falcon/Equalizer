#pragma once

#include "Filter.h"
#include "fdacoefs.h"

class IIR : public Filter
{
public:
	IIR();
	~IIR() {}
private:
	void HandleData();
};