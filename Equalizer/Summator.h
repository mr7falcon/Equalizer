#pragma once

#include "DataHandler.h"

class Summator : public DataHandler
{
public:
	Summator(const unsigned short numOfBands);
	~Summator();

private:
	void HandleEvent();

	void Reallocate();

	const unsigned short numOfBands;

	short* m_summ;
	unsigned short m_numOfChunks;
};