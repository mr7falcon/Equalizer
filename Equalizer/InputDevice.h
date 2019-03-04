#pragma once

#include <iostream>
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

	FILE* file;

	const unsigned short defaultChunkSize = 65000;

	WaveFormat header;
};