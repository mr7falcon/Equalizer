#pragma once

#include "Block.h"

class DataHandler : public Block
{
public:
	DataHandler();
	~DataHandler();

	void SendNewData(DataChunk* newCurrentData);
	bool IsProcessingData() { return m_currentData != nullptr; }

	void SetOutput(Block* output) { this->output = output; }
protected:
	Block* output;

	std::mutex g_dataLock;
	std::condition_variable g_dataProcessed;
};