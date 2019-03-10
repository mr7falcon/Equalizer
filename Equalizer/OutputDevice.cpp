#include "OutputDevice.h"

OutputDevice::OutputDevice()
	:m_pDS(NULL),
	m_bufferSize(defaultChunkSize),
	m_rdPos(0),
	m_wtPos(0),
	m_playingAllowed(false)
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

	if (FAILED(hr = m_pDS->SetCooperativeLevel(hDlg, DSSCL_NORMAL)))
		return hr;

	return S_OK;
}

HRESULT OutputDevice::FreeDirectSound()
{
	for (unsigned int i = 0; i < buffersCount; ++i)
		SAFE_RELEASE(m_buffers[i].first);
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

	m_buffers.push_back(std::make_pair(lpDSBuf, false));

	return S_OK;
}

HRESULT OutputDevice::FillBuffer()
{
	if (m_buffers[m_wtPos].first && !m_buffers[m_wtPos].second)
	{
		HRESULT hr;

		void* pbData = nullptr;
		void* pbData2 = nullptr;
		unsigned long dwLength;
		unsigned long dwLength2;

		if (FAILED(hr = m_buffers[m_wtPos].first->Lock(0, m_bufferSize, &pbData, &dwLength,
			&pbData2, &dwLength2, 0L)))
			return hr;

		memcpy(pbData, m_currentData->data, m_currentData->size);

		m_buffers[m_wtPos].first->Unlock(pbData, m_bufferSize, NULL, 0);
		pbData = NULL;

		m_buffers[m_wtPos].second = true;

		delete(m_currentData);
		m_currentData = nullptr;

		m_wtPos = ++m_wtPos % buffersCount;

		return S_OK;
	}

	return S_FALSE;
}

bool OutputDevice::IsPlaying()
{
	for (int i = 0; i < buffersCount; ++i)
	{
		if (m_buffers[i].first)
		{
			unsigned long dwStatus = 0;
			m_buffers[i].first->GetStatus(&dwStatus);

			if (dwStatus & DSBSTATUS_PLAYING)
				return true;
		}
	};

	return false;
}

HRESULT OutputDevice::Play()
{
	HRESULT hr;

// 	if (FAILED(hr = RestoreBuffers()))
// 		return hr;

	if (FAILED(hr = m_buffers[m_rdPos].first->Play(0, 0, 0)))
		return hr;

	//Log("Chunk played");

	m_buffers[m_rdPos].second = false;

	m_rdPos = ++m_rdPos % buffersCount;

	output->SendNewData(new DataChunk);

	return S_OK;
}

HRESULT OutputDevice::RestoreBuffers()
{
	HRESULT hr;

	
	if (NULL == m_buffers[m_rdPos].first)
		return S_OK;

	unsigned long dwStatus;
	if (FAILED(hr = m_buffers[m_rdPos].first->GetStatus(&dwStatus)))
		return hr;

	if (dwStatus & DSBSTATUS_BUFFERLOST)
	{
		do
		{
			hr = m_buffers[m_rdPos].first->Restore();
			if (hr == DSERR_BUFFERLOST)
				Sleep(10);
		} while (hr = m_buffers[m_rdPos].first->Restore());

		if (FAILED(hr = FillBuffer()))
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

void OutputDevice::HandleData()
{
	//Log("New data chunk received");

	if (FAILED(FillBuffer()))
	{
		Log("buffer filling error");
		delete(m_currentData);
		return;
	}

	//Log("Buffer filled");
}

void OutputDevice::StartPlaying()
{
	while (true)
	{
		if (m_playingAllowed)
		{
			if (!IsPlaying())
			{
				if (m_buffers[m_rdPos].second)
				{
					if (FAILED(Play()))
					{
						Log("buffer playing error");
						delete(m_currentData);
						return;
					}
				}
			}
		}
	}
}