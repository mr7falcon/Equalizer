#pragma once

#include "DataHandler.h"

enum Effects : unsigned short
{
	EFFECT_DELAY,
	EFFECT_CLIPPING
};

class Summator : public DataHandler
{
public:
	Summator(const unsigned short numOfBands);
	~Summator();

	void SetEffect(Effects effect, bool reset = false);

private:
	void HandleEvent();

	void Reallocate();

	void ApplyDelay();
	void ApplyClipping();

	const unsigned short numOfBands;

	short* m_summ;
	unsigned short m_numOfChunks;

	bool m_delay;
	bool m_clipping;
	const unsigned short m_sampleDelay;
	short* m_prevLastCounts;
};