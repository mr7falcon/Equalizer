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
	const unsigned long dataSize = defaultChunkSize * 2;
	byte* data = new byte[dataSize];

	if (m_file.tellg() >= m_header.chunkSize)
	{
		m_file.clear();
		m_file.seekg(sizeof(m_header));
	}
	
	m_file.read((char*)data, dataSize);

	DataChunk* newChunk = DecodeChunk(data);

	return newChunk;
}

void InputDevice::CloseFile()
{
	m_file.close();
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
	m_currentData = FillChunk();

	for (int i = 0; i < outputs.size(); ++i)
	{
		if (outputs[i])
		{
			dynamic_cast<DataHandler*>(outputs[i])->SendNewData(m_currentData);
		}
	}
}

DataChunk* InputDevice::DecodeChunk(const byte* inputData)
{
	DataChunk* newData = new DataChunk(defaultChunkSize);

	unsigned long j = 0;
	for (unsigned long i = 0; i < newData->size; ++i)
	{
		newData->data[i] = ((short int)inputData[j + 1] << 8) | inputData[j];
		j += 2;
	}

	return newData;
}