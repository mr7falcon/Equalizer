#pragma once

#include <fstream>
#include <vector>
#include "Block.h"
#include "DataHandler.h"

class InputDevice : public Block
{
public:
	InputDevice();
	~InputDevice();

	bool OpenFile(const char* fileName, WAVEFORMATEX& waveFormat);

	void CloseFile();

	void SetOutput(Block* output) { outputs.push_back(output); };

private:
	const short* FillChunk();

	void HandleEvent();
	void HandleNewDataRequested();

	const short* DecodeChunk(const byte* inputData = nullptr);

	std::ifstream file;

	WaveFormat header;

	std::vector<Block*> outputs;
};