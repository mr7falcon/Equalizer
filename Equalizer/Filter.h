#pragma once

#include "Block.h"

class CQueue
{
public:
	CQueue(const int size);
	~CQueue() { delete[](buffer); }

	unsigned short operator[](const int i) { return buffer[i]; }

	void Enqueue(const unsigned short a);
	unsigned short Dequeue();

	bool IsEmpty() const { return head == tail; }

private:
	unsigned short* buffer;
	const int bufferSize;
	int head;
	int tail;
};

class Filter : public Block
{
public:
	Filter();
	~Filter() { delete(counts); }

private:
	static const unsigned short order = 24;

	CQueue* counts;
};