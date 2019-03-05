#include "OutputDevice.h"

OutputDevice::OutputDevice()
	:Block(NULL),
	m_pDS(NULL),
	m_bufferSize(defaultChunkSize),
	m_rdPos(0),
	m_wtPos(0),
	inputDevice(nullptr)
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
	for (unsigned int i = 0; i < m_bufferSize; ++i)
		SAFE_RELEASE(m_buffers[i]);
	SAFE_RELEASE(m_pDS);

	CoUninitialize();

	return S_OK;
}

HRESULT OutputDevice::CreateBuffer(WAVEFORMATEX& waveFormat)
{
	HRESULT hr;
	LPDIRECTSOUNDBUFFER lpDSBuf;

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = m_bufferSize;
	dsbd.lpwfxFormat = dynamic_cast<LPWAVEFORMATEX>(&waveFormat);
 
	if (FAILED(hr = m_pDS->CreateSoundBuffer(&dsbd, &lpDSBuf, NULL)))
		return hr;

	m_buffers.push_back(lpDSBuf);

	return S_OK;
}

HRESULT OutputDevice::FillBuffer(DataChunk* data)
{
	if (m_buffers[m_wtPos])
	{
		HRESULT hr;

		void* pbData = nullptr;
		void* pbData2 = nullptr;
		unsigned long dwLength;
		unsigned long dwLength2;

		g_lock.lock();

		if (FAILED(hr = m_buffers[m_wtPos]->Lock(0, m_bufferSize, &pbData, &dwLength,
			&pbData2, &dwLength2, 0L)))
			return hr;

		memcpy(pbData, data->data, data->size);

		m_buffers[m_wtPos]->Unlock(pbData, m_bufferSize, NULL, 0);
		pbData = NULL;

		m_wtPos++;
		Circle();

		g_lock.unlock();

		return S_OK;
	}

	return S_FALSE;
}

bool OutputDevice::IsPlaying()
{
	for (int i = 0; i < buffersCount; ++i)
	{
		if (m_buffers[i])
		{
			unsigned long dwStatus = 0;
			m_buffers[i]->GetStatus(&dwStatus);

			if (dwStatus & DSBSTATUS_PLAYING)
				return true;
		}
	};

	return false;
}

HRESULT OutputDevice::Play(DataChunk* data)
{
	HRESULT hr;

	if (FAILED(hr = RestoreBuffers(data)))
		return hr;

	if (m_buffers[m_rdPos])
	{
		g_lock.lock();

		if (FAILED(hr = m_buffers[m_rdPos]->Play(0, 0, 0)))
			return hr;

		SAFE_RELEASE(m_buffers[m_rdPos]);

		m_rdPos++;
		Circle();

		g_lock.unlock();

		RequestNewDataChunk(inputDevice);

		return S_OK;
	}

	return S_FALSE;
}

HRESULT OutputDevice::RestoreBuffers(DataChunk* data)
{
	HRESULT hr;

	
	if (NULL == m_buffers[m_rdPos])
		return S_OK;

	unsigned long dwStatus;
	if (FAILED(hr = m_buffers[m_rdPos]->GetStatus(&dwStatus)))
		return hr;

	if (dwStatus & DSBSTATUS_BUFFERLOST)
	{
		do
		{
			hr = m_buffers[m_rdPos]->Restore();
			if (hr == DSERR_BUFFERLOST)
				Sleep(10);
		} while (hr = m_buffers[m_rdPos]->Restore());

		if (FAILED(hr = FillBuffer(data)))
			return hr;
	}

	return S_OK;
}

HRESULT OutputDevice::Init(HWND hDlg, WAVEFORMATEX& waveFormat)
{
	HRESULT hr;

	if (FAILED(hr = InitDevice(hDlg)))
		return hr;

	for (int i = 0; i < buffersCount; ++i)
	{
		if (FAILED(hr = CreateBuffer(waveFormat)))
			return hr;
	}

	return S_OK;
}

void OutputDevice::Circle()
{
	if (m_rdPos >= buffersCount)
		m_rdPos -= buffersCount;

	if (m_wtPos >= buffersCount)
		m_wtPos -= buffersCount;
}

void OutputDevice::HandleDataInternal(DataChunk* data)
{
	Log("New data chunk received");

	if (!IsBuffersFull())
	{
		if (FAILED(FillBuffer(data)))
		{
			Log("buffer filling error");
			delete(data);
			return;
		}

		Log("Buffer filled");

		if (!IsPlaying())
		{
			if (FAILED(Play(data)))
			{
				Log("buffer playing error");
				delete(data);
				return;
			}
		}

		Log("Chunk played");
	}

	delete(data);
}

bool OutputDevice::IsBuffersFull() const
{
	for (int i = 0; i < buffersCount; ++i)
	{
		if (m_buffers[i])
			return false;
	}

	return true;
}