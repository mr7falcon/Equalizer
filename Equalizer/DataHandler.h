#pragma once

#include "Block.h"
#include <mutex>
#include <condition_variable>

extern std::mutex g_lock;
extern std::condition_variable g_dataProcessed;

class DataHandler : public Block
{
public:
	DataHandler();
	~DataHandler();

	void SendNewData(DataChunk* newCurrentData);

	void SetOutput(Block* output) { this->output = output; }
protected:
	Block* output;
};