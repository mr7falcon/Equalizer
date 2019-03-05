#pragma once

#include <dsound.h>
#include "WaveFormat.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

class OutputDevice
{
public:
	OutputDevice();
	~OutputDevice();

	HRESULT PlayChunk(HWND hDlg, DataChunk* data);
	HRESULT Init(HWND hDlg, WAVEFORMATEX& waveFormat);
	bool IsBufferPlaying();

private:
	HRESULT InitDevice(HWND hDlg);
	HRESULT FreeDirectSound();
	HRESULT CreateBuffer(WAVEFORMATEX& waveFormat);
	HRESULT FillBuffer(DataChunk* data);

	HRESULT PlayBuffer(bool bLooped, DataChunk* data);
	HRESULT RestoreBuffers(DataChunk* data);

	LPDIRECTSOUND m_pDS;
	LPDIRECTSOUNDBUFFER m_pDSBuffer;

	unsigned long m_bufferSize;
};