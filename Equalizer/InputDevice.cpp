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
		std::cout << "file opening error" << std::endl;
		return false;
	}

	file.read((char*)&header, sizeof(header));

	waveFormat.cbSize = sizeof(WAVEFORMATEX);
	waveFormat.nChannels = header.numChannels;
	waveFormat.nSamplesPerSec = header.sampleRate;
	waveFormat.wBitsPerSample = header.bitsPerSample;
	waveFormat.nAvgBytesPerSec = header.byteRate;
	waveFormat.nBlockAlign = header.blockAlign;
	waveFormat.wFormatTag = header.audioFormat;

	return true;
}

DataChunk* InputDevice::FillChunk()
{
	DataChunk* data = new DataChunk(defaultChunkSize);

	file.read((char*)data->data, defaultChunkSize);

	return data;
}

DataChunk* InputDevice::GetNextChunk()
{
	return FillChunk();
}
void InputDevice::CloseFile()
{
	file.close();
}