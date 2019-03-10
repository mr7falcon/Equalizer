#include "Filter.h"

CQueue::CQueue(const int size)
	:bufferSize(size),
	head(0),
	tail(0)
{
	buffer = new unsigned short[bufferSize];
}

void CQueue::Enqueue(const unsigned short a)
{
	if ((tail + 1) % bufferSize != head)
	{
		buffer[tail] = a;
		tail = ++tail % bufferSize;
	}
}

unsigned short CQueue::Dequeue()
{
	if (head != tail)
	{
		unsigned short res = buffer[head];
		head = ++head % bufferSize;

		return res;
	}

	return 0;
}

Filter::Filter()
{
	counts = new CQueue(order);
}