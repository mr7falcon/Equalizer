#pragma once

#include "Block.h"

extern std::mutex g_dataLock;
extern std::condition_variable g_dataProcessed;

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
};