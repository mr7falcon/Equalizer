#pragma once

#include "DataHandler.h"
#include "fftw3.h"
#include <cmath>

enum Effects : unsigned short
{
	EFFECT_DELAY,
	EFFECT_CLIPPING,
	EFFECT_SCLIPPING
};

const double pi = 3.1415926535;

class Summator : public DataHandler
{
public:
	Summator(const unsigned short numOfBands);
	~Summator();

	void SetEffect(Effects effect, bool reset = false);

	void SetClippingCoef(const short coef);
	const short GetClippingCoef() const { return clippingCoef; }

private:
	void HandleEvent();

	void Reallocate();

	void ApplyDelay();
	void ApplyClipping();
	void ApplySClipping();

	const unsigned short numOfBands;

	short* sum;
	unsigned short numOfChunks;

	bool delay;
	bool clipping;
	bool sclipping;

	const unsigned short sampleDelay;
	short* prevLastCounts;

	short maxClippingAmp;
	long maxSClippingAmp;
	short clippingCoef;
	const short standartMaxClippingAmp;
	const long standartMaxSClippingAmp;
};