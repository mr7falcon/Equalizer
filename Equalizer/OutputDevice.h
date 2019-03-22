#pragma once

#include <vector>
#include "DataHandler.h"
#include "InputDevice.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } } 

const unsigned short sectionCount = 4;

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
	HRESULT FillBuffer(const unsigned long startPos);

	HRESULT Play();

	void HandleEvent();
	void HandleNewDataReceived();
	void HandleSectionPlayed();

	byte* EncodeChunk(const DataChunk* newChunk = nullptr);

	LPDIRECTSOUND m_pDS;

	unsigned long m_bufferSize;
	LPDIRECTSOUNDBUFFER m_buffer;

	byte* m_encodedCurrentData;

	unsigned short m_currentSection;

	bool m_playingAllowed;

	HANDLE* bufferEvents;

	void InitBufferEvents();
};