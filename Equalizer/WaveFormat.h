#pragma once

#include <dsound.h>

struct WaveFormat
{
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

	DataChunk(unsigned short size)
		:size(size)
	{
		data = new byte[size];
	}

	~DataChunk()
	{
		delete(data);
	}

	unsigned short size;
	byte* data;
};

const unsigned short defaultChunkSize = 65500;