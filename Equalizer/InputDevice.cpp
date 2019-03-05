#include "InputDevice.h"

InputDevice::InputDevice()
{
}

InputDevice::~InputDevice()
{
}

bool InputDevice::OpenFile(const char* fileName, WAVEFORMATEX& waveFormat)
{
	m_file.open(fileName, std::fstream::binary);

	if (!m_file.is_open())
	{
		std::cout << "file opening error" << std::endl;
		return false;
	}

	m_file.read((char*)&m_header, sizeof(m_header));

	if (!m_header.Check())
	{
		std::cout << "file format error" << std::endl;
		return false;
	}

	waveFormat.cbSize = sizeof(WAVEFORMATEX);
	waveFormat.nChannels = m_header.numChannels;
	waveFormat.nSamplesPerSec = m_header.sampleRate;
	waveFormat.wBitsPerSample = m_header.bitsPerSample;
	waveFormat.nAvgBytesPerSec = m_header.byteRate;
	waveFormat.nBlockAlign = m_header.blockAlign;
	waveFormat.wFormatTag = m_header.audioFormat;

	return true;
}

DataChunk* InputDevice::FillChunk()
{
	DataChunk* data = new DataChunk(defaultChunkSize);

	m_file.read((char*)data->data, defaultChunkSize);

	return data;
}

DataChunk* InputDevice::GetNextChunk()
{
	return FillChunk();
}
void InputDevice::CloseFile()
{
	m_file.close();
}