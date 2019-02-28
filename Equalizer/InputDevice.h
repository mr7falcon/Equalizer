#pragma once

#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

class InputDevice
{
public:
	InputDevice();
	~InputDevice();

	HRESULT Open(const char* strFilename);
	HRESULT Reset();
	HRESULT Read(unsigned int nSizeToRead, const byte pbData, unsigned int& pnSizeRead);
	HRESULT Close();

private:
	HRESULT WaveOpenFile(const char* strFileName);
	HRESULT ReadMMIO();

	HMMIO m_hmmioIn;
	WAVEFORMATEX* m_pwfx;
	MMCKINFO m_ckInRiff;
};