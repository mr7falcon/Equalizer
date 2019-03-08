#pragma once

#include <vector>
#include "InputDevice.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } } 

static const unsigned short buffersCount = 3;

class OutputDevice : public Block
{
public:
	OutputDevice();
	~OutputDevice();

	HRESULT Init(HWND hDlg, WAVEFORMATEX& waveFormat);
	bool IsPlaying();
	
	void StartPlaying();

private:
	HRESULT InitDevice(HWND hDlg);
	HRESULT FreeDirectSound();
	HRESULT CreateBuffer(WAVEFORMATEX& waveFormat);
	HRESULT FillBuffer();

	HRESULT Play();
	HRESULT RestoreBuffers();
//	bool IsBuffersFull() const;

	void HandleData();

	LPDIRECTSOUND m_pDS;

	unsigned long m_bufferSize;

	std::vector<LPDIRECTSOUNDBUFFER> m_buffers;
	unsigned short m_rdPos;
	unsigned short m_wtPos;
	void Circle();
};