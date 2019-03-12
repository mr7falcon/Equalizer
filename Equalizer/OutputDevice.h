#pragma once

#include <vector>
#include "DataHandler.h"
#include "InputDevice.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } } 

class OutputDevice : public DataHandler 
{
public:
	OutputDevice();
	~OutputDevice();

	HRESULT Init(HWND hDlg, WAVEFORMATEX& waveFormat);
	bool IsPlaying();
	
	void StartPlaying();
	void SetPlayingAllowed(bool allowed) { m_playingAllowed = allowed; }

private:
	HRESULT InitDevice(HWND hDlg);
	HRESULT FreeDirectSound();
	HRESULT CreateBuffer(WAVEFORMATEX& waveFormat);
	HRESULT FillBuffer(const unsigned long startPos, const unsigned long size);

	HRESULT Play();

	void HandleEvent();
	void HandleNewDataReceived();
	void HandleFirstHalfBufferPlayed();
	void HandleSecondHalfBufferPlayed();

	LPDIRECTSOUND m_pDS;

	unsigned long m_bufferSize;
	LPDIRECTSOUNDBUFFER m_buffer;
	bool m_bufferFilled;

	bool m_playingAllowed;
};