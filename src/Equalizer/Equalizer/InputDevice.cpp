#include "InputDevice.h"

InputDevice::InputDevice()
{
}

InputDevice::~InputDevice()
{
}

bool InputDevice::OpenFile(const char* fileName, WAVEFORMATEX& waveFormat)
{
	file.open(fileName, std::fstream::binary);

	if (!file.is_open())
	{
		Log("file opening error");
		return false;
	}

	file.read((char*)&header, sizeof(header));

	if (!header.Check())
	{
		Log("file format error");
		return false;
	}

	waveFormat.cbSize = sizeof(WAVEFORMATEX);
	waveFormat.nChannels = header.numChannels;
	waveFormat.nSamplesPerSec = header.sampleRate;
	waveFormat.wBitsPerSample = header.bitsPerSample;
	waveFormat.nAvgBytesPerSec = header.byteRate;
	waveFormat.nBlockAlign = header.blockAlign;
	waveFormat.wFormatTag = header.audioFormat;

	return true;
}

const short* InputDevice::FillChunk()
{
	const unsigned long dataSize = defaultChunkSize * sizeof(short);
	byte* data = new byte[dataSize];

	if (file.tellg() >= header.chunkSize)
	{
		file.clear();
		file.seekg(sizeof(header));
	}
	
	file.read((char*)data, dataSize);

	const short* newChunk = DecodeChunk(data);

	return newChunk;
}

void InputDevice::CloseFile()
{
	file.close();
}

void InputDevice::HandleEvent()
{
	switch (event)
	{
	case EVENT_NEW_DATA_REQUESTED:
		HandleNewDataRequested();
		break;
	}
}

void InputDevice::HandleNewDataRequested()
{
	currentData = FillChunk();

	for (int i = 0; i < outputs.size(); ++i)
	{
		if (outputs[i])
		{
			dynamic_cast<DataHandler*>(outputs[i])->SendNewData(currentData);
		}
	}
}

const short* InputDevice::DecodeChunk(const byte* inputData)
{
	short* newData = new short[defaultChunkSize];

	unsigned long j = 0;
	for (unsigned long i = 0; i < defaultChunkSize; ++i)
	{
		newData[i] = ((short int)inputData[j + 1] << 8) | inputData[j];
		j += 2;
	}

	return newData;
}