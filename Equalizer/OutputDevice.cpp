#include "OutputDevice.h"

OutputDevice::OutputDevice()
	:m_pDS(NULL),
	m_pDSBuffer(NULL)
{
}

OutputDevice::~OutputDevice()
{
}

HRESULT OutputDevice::InitDevice(HWND hDlg)
{
	HRESULT hr;
	LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

	if (hr = CoInitialize(NULL))
		return hr;

	if (FAILED(hr = DirectSoundCreate(NULL, &m_pDS, NULL)))
		return hr;

	if (FAILED(hr = m_pDS->SetCooperativeLevel(hDlg, DSSCL_PRIORITY)))
		return hr;

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	if (FAILED(hr = m_pDS->CreateSoundBuffer(&dsbd, &pDSBPrimary, NULL)))
		return hr;

	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 44100;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	if (FAILED(hr = pDSBPrimary->SetFormat(&wfx)))
		return hr;
		
	SAFE_RELEASE(pDSBPrimary);

	return S_OK;
}

HRESULT OutputDevice::FreeDirectSound()
{
	SAFE_RELEASE(m_pDSBuffer);
	SAFE_RELEASE(m_pDS);

	CoUninitialize();

	return S_OK;
}

HRESULT OutputDevice::CreateBuffer(WAVEFORMATEX& waveFormat)
{
	HRESULT hr;

	SAFE_RELEASE(m_pDSBuffer);

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = defaultChunkSize;
	dsbd.lpwfxFormat = dynamic_cast<LPWAVEFORMATEX>(&waveFormat);
 
	if (FAILED(hr = m_pDS->CreateSoundBuffer(&dsbd, &m_pDSBuffer, NULL)))
		return hr;

	m_bufferSize = dsbd.dwBufferBytes;

	return S_OK;
}

HRESULT OutputDevice::FillBuffer(DataChunk* data)
{
	HRESULT hr;
	
	void* pbData = nullptr;
	void* pbData2 = nullptr;
	unsigned long dwLength;
	unsigned long dwLength2;

	if (FAILED(hr = m_pDSBuffer->Lock(0, m_bufferSize, &pbData, &dwLength,
		&pbData2, &dwLength2, 0L)))
		return hr;

	memcpy(pbData, data->data, data->size);

	m_pDSBuffer->Unlock(pbData, m_bufferSize, NULL, 0);
	pbData = NULL;
 
	return S_OK;
}

bool OutputDevice::IsBufferPlaying()
{
	unsigned long dwStatus = 0;

	if (NULL == m_pDSBuffer)
		return false;

	m_pDSBuffer->GetStatus(&dwStatus);

	if (dwStatus & DSBSTATUS_PLAYING)
		return true;
	else
		return false;
}

HRESULT OutputDevice::PlayBuffer(bool bLooped, DataChunk* data)
{
	HRESULT hr;

	if (NULL == m_pDSBuffer)
		return E_FAIL;

	if (FAILED(hr = RestoreBuffers(data)))
		return hr;

	unsigned long dwLooped = bLooped ? DSBPLAY_LOOPING : 0L;
	if (FAILED(hr = m_pDSBuffer->Play(0, 0, dwLooped)))
		return hr;

	delete(data);

	return S_OK;
}

HRESULT OutputDevice::RestoreBuffers(DataChunk* data)
{
	HRESULT hr;

	if (NULL == m_pDSBuffer)
		return S_OK;

	unsigned long dwStatus;
	if (FAILED(hr = m_pDSBuffer->GetStatus(&dwStatus)))
		return hr;

	if (dwStatus & DSBSTATUS_BUFFERLOST)
	{
		do
		{
			hr = m_pDSBuffer->Restore();
			if (hr == DSERR_BUFFERLOST)
				Sleep(10);
		} while (hr = m_pDSBuffer->Restore());

		if (FAILED(hr = FillBuffer(data)))
			return hr;
	}

	return S_OK;
}

HRESULT OutputDevice::PlayChunk(HWND hDlg, DataChunk* data)
{
	HRESULT hr;

	if (FAILED(hr = FillBuffer(data)))
		return hr;

	if (FAILED(hr = PlayBuffer(false, data)))
		return hr;
	
	return S_OK;
}

HRESULT OutputDevice::Init(HWND hDlg, WAVEFORMATEX& waveFormat)
{
	HRESULT hr;

	if (FAILED(hr = InitDevice(hDlg)))
		return hr;

	if (FAILED(hr = CreateBuffer(waveFormat)))
		return hr;

	return S_OK;
}