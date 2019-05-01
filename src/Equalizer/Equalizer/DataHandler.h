#pragma once

#include "Block.h"

class DataHandler : public Block
{
public:
	DataHandler();
	~DataHandler();

	void SendNewData(const short* newCurrentData);
	bool IsProcessingData() { return currentData != nullptr; }

	void SetOutput(Block* output) { this->output = output; }
protected:
	Block* output;

	std::mutex dataLock;
	std::condition_variable dataProcessed;
};