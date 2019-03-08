#pragma once

#include <fstream>
#include "Block.h"

class InputDevice : public Block
{
public:
	InputDevice();
	~InputDevice();

	bool OpenFile(const char* fileName, WAVEFORMATEX& waveFormat);

	void CloseFile();

private:
	DataChunk* FillChunk();

	void HandleData();

	std::ifstream m_file;

	WaveFormat m_header;
};