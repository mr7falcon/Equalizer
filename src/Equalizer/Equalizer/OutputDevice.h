#pragma once

#include <string>
#include "DataHandler.h"
#include "InputDevice.h"

const unsigned short sectionCount = 4;

class SwapBuffer
{
public:
	SwapBuffer();
	~SwapBuffer();

	const byte* ReadSwapSection();
	void WriteSwapSection(const byte* data);

	const unsigned short GetReadPos() const { return rdpos; }
	const unsigned short GetWritePos() const { return wtpos; }

	bool IsFull() const;

private:
	const byte** swapData;
	unsigned short rdpos;
	unsigned short wtpos;
};

class OutputDevice : public DataHandler 
{
public:
	OutputDevice();
	~OutputDevice();

	HRESULT Init(HWND hDlg, WAVEFORMATEX& waveFormat);
	bool IsPlaying();
	
	void StartPlaying();

	HRESULT Play();

	HRESULT Stop();

private:
	HRESULT InitDevice(HWND hDlg);
	HRESULT FreeDirectSound();
	HRESULT CreateBuffer(WAVEFORMATEX& waveFormat);
	HRESULT FillBuffer(const unsigned long startPos, const byte* data);
	HRESULT FillBufferSection(const byte* data);

	void HandleEvent();
	void HandleNewDataReceived();
	void HandleSectionPlayed();

	const byte* EncodeChunk(const short* newChunk = nullptr);

	LPDIRECTSOUND pDS;

	unsigned long bufferSize;
	LPDIRECTSOUNDBUFFER buffer;
	SwapBuffer swapBuffer;
	unsigned short currentSection;

	std::mutex playingLock;
	std::condition_variable allowPlaying;
	bool playingAllowed;

	HANDLE* bufferEvents;

	void InitBufferEvents();
};