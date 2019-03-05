#pragma once

#include <fstream>
#include "Block.h"

class InputDevice : public Block
{
public:
	InputDevice(Block* output);
	~InputDevice();

	bool OpenFile(const char* fileName, WAVEFORMATEX& waveFormat);

	void CloseFile();

	void OnNewChunkRequested();
	void HandleDataInternal(DataChunk* data) { return; }

private:
	DataChunk* FillChunk();

	std::ifstream m_file;

	WaveFormat m_header;
};

void RequestNewDataChunk(InputDevice* inputDevice);