#include "InputDevice.h"

InputDevice::InputDevice()
	:file(nullptr)
{
}

InputDevice::~InputDevice()
{
}

bool InputDevice::OpenFile(const char* fileName, WAVEFORMATEX& waveFormat)
{
	fopen_s(&file, fileName, "r");

	if (!file)
	{
		std::cout << "file opening error" << std::endl;
		return false;
	}

	std::fread(&header, sizeof(header), 1, file);

	waveFormat.cbSize = defaultChunkSize;
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

	std::fread(data->data, defaultChunkSize, 1, file);

	return data;
}

DataChunk* InputDevice::GetNextChunk()
{
	return FillChunk();
}
void InputDevice::CloseFile()
{
	std::fclose(file);
}