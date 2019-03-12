// #include "Filter.h"
// 
// // CQueue::CQueue(const int size)
// // 	:bufferSize(size),
// // 	head(0),
// // 	tail(0)
// // {
// // 	buffer = new unsigned short[bufferSize];
// // }
// // 
// // void CQueue::Enqueue(const unsigned short a)
// // {
// // 	if ((tail + 1) % bufferSize != head)
// // 	{
// // 		buffer[tail] = a;
// // 		tail = ++tail % bufferSize;
// // 	}
// // }
// // 
// // unsigned short CQueue::Dequeue()
// // {
// // 	if (head != tail)
// // 	{
// // 		unsigned short res = buffer[head];
// // 		head = ++head % bufferSize;
// // 
// // 		return res;
// // 	}
// // 
// // 	return 0;
// // }
// 
// Filter::Filter()
// {
// }
// 
// void Filter::Init(WAVEFORMATEX& format)
// {
// 	bitsPerSample = format.wBitsPerSample;
// }
// 
// const int Filter::DecodeChunk(short int** counts)
// {
// 	if (bitsPerSample > 8 && bitsPerSample <= 16)
// 	{
// 		int size = m_currentData->size / sizeof(short int);
// 		*counts = new short int[size];
// 
// 		for (int i = 0; i < size; ++i)
// 		{
// 			unsigned long j = 0;
// 			*counts[i] = (short int)(m_currentData->data[j + 1] << 8 | m_currentData->data[j]);
// 			++j;
// 		}
// 	}
// 
// 	delete(m_currentData);
// }
// 
// void Filter::Transforming(const int size, short int* counts) const
// {
// 	fftw_plan plan = fftw_plan_dft_1d(size, (fftw_complex*)(&counts[0]), (fftw_complex*)(&counts[size - 1]), FFTW_FORWARD, FFTW_ESTIMATE);
// 	fftw_execute(plan);
// 	fftw_destroy_plan(plan);
// }