// #pragma once
// 
// #include "Block.h"
// #include "fftw3.h"
// 
// // class CQueue
// // {
// // public:
// // 	CQueue(const int size);
// // 	~CQueue() { delete[](buffer); }
// // 
// // 	unsigned short operator[](const int i) { return buffer[i]; }
// // 
// // 	void Enqueue(const unsigned short a);
// // 	unsigned short Dequeue();
// // 
// // 	bool IsEmpty() const { return head == tail; }
// // 
// // private:
// // 	unsigned short* buffer;
// // 	const int bufferSize;
// // 	int head;
// // 	int tail;
// // };
// 
// class Filter : public Block
// {
// public:
// 	Filter();
// 	~Filter() {}
// 
// 	void Init(WAVEFORMATEX& format);
// 
// private:
// 	static const unsigned short order = 24;
// 	static unsigned short bitsPerSample;
// 
// 	const int DecodeChunk(short int** counts = nullptr);
// 	void Transforming(int size, short int* counts) const;
// 	virtual void Filtering(int size, const short int* counts, short int** filtredCounts = nullptr) const = 0;
// };