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
		Log("file opening error");
		return false;
	}

	m_file.read((char*)&m_header, sizeof(m_header));

	if (!m_header.Check())
	{
		Log("file format error");
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

	if (m_file.eof())
	{
		m_file.clear();
		m_file.seekg(0);
	}
	
	m_file.read((char*)data->data, defaultChunkSize);

	return data;
}

void InputDevice::CloseFile()
{
	m_file.close();
}

void InputDevice::HandleData()
{
	delete(m_newData);
	m_newData = nullptr;

	m_currentData = FillChunk();
	output->SendNewData(m_currentData);
	m_currentData = nullptr;
}