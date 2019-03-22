#pragma once

#include "DataHandler.h"

class Summator : public DataHandler
{
public:
	Summator(const unsigned short numOfBands);
	~Summator();

private:
	void HandleEvent();

	const unsigned short numOfBands;

	DataChunk* m_summ;
	unsigned short m_numOfChunks;
};