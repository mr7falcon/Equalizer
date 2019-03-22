 #pragma once
 
#include "DataHandler.h"
#include "fftw3.h"
#include "fcoefs.h"

 
//   class CQueue
//   {
//   public:
//   	CQueue(const int size);
//   	~CQueue() { delete[](buffer); }
//   
//   	unsigned short operator[](const int i) { return buffer[i]; }
//   
//   	void Enqueue(const unsigned short a);
//   	unsigned short Dequeue();
//   
//   	bool IsEmpty() const { return head == tail; }
//   
//   private:
//   	unsigned short* buffer;
//   	const int bufferSize;
//   	int head;
//   	int tail;
//   };
  
 class Filter : public DataHandler
 {
 public:
 	Filter(const unsigned short num);
	~Filter();
 
 	void Init(unsigned short bitsPerSample);

	void SetGain(float mult);
	float GetGain() const;
 
 protected:
	const unsigned short num;
	unsigned short order;
 	unsigned short bitsPerSample;
 
 	//short int* Transform(const unsigned long size, short int* counts, bool backward = false) const;
 	virtual short* Filtering() = 0;

	void HandleEvent();

	short* m_prevLastCounts;

	float m_gain;
 };