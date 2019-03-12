#pragma once

#include "Block.h"

class DataHandler : public Block
{
public:
	DataHandler();
	~DataHandler();

	void SendNewData(DataChunk* newCurrentData);
protected:
	void ApplyNewData();

	DataChunk* m_newData;
};