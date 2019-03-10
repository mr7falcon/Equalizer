#pragma once

struct WaveFormat
{
	WaveFormat()
	{
		for (int i = 0; i < 4; ++i)
		{
			chunkId[i] = NULL;
			format[i] = NULL;
			subchunk1Id[i] = NULL;
			subchunck2Id[i] = NULL;
		}
		chunkSize = NULL;
		subchunk1Size = NULL;
		audioFormat = NULL;
		numChannels = NULL;
		sampleRate = NULL;
		byteRate = NULL;
		blockAlign = NULL;
		bitsPerSample = NULL;
		subchunk2Size = NULL;
	}
	
	bool Check() const
	{
		for (int i = 0; i < 4; ++i)
		{
			if (!chunkId[i] || !format[i] || !subchunk1Id[i] || !subchunck2Id[i])
				return false;
		}
		return chunkSize && subchunk1Size && subchunk2Size && audioFormat && numChannels && sampleRate && byteRate && blockAlign && bitsPerSample;
	}

	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subchunk1Id[4];
	unsigned long subchunk1Size;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long byteRate;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char subchunck2Id[4];
	unsigned long subchunk2Size;
};

typedef unsigned char byte;

struct DataChunk
{
	DataChunk()
		:size(0),
		data(nullptr)
	{}

	DataChunk(unsigned long size)
		:size(size)
	{
		data = new byte[size];
	}

	~DataChunk()
	{
		delete[](data);
	}

	unsigned long size;
	byte* data;
};

const unsigned long defaultChunkSize = 524288;