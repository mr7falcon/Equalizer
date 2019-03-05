#pragma once

#include <iostream>
#include <fstream>
#include <dsound.h>
#include "WaveFormat.h"

class InputDevice
{
public:
	InputDevice();
	~InputDevice();

	bool OpenFile(const char* fileName, WAVEFORMATEX& waveFormat);

	DataChunk* GetNextChunk();

	void CloseFile();

private:
	DataChunk* FillChunk();

	std::ifstream m_file;

	WaveFormat m_header;
};