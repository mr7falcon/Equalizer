#include "OutputDevice.h"

OutputDevice::OutputDevice()
	:m_pDS(NULL),
	m_bufferSize(defaultChunkSize),
	m_buffer(nullptr),
	m_playingAllowed(false),
	m_bufferFilled(false)
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
	SAFE_RELEASE(m_buffer);
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

	m_buffer = lpDSBuf;

	return S_OK;
}

HRESULT OutputDevice::FillBuffer(const unsigned long startPos, const unsigned long size)
{
	if (m_buffer && m_currentData)
	{
		HRESULT hr;

		void* pbData = nullptr;
		void* pbData2 = nullptr;
		unsigned long dwLength;
		unsigned long dwLength2;

		if (FAILED(hr = m_buffer->Lock(startPos, size, &pbData, &dwLength,
			&pbData2, &dwLength2, 0L)))
			return hr;

		void* data = m_currentData->data + startPos;
		memcpy(pbData, data, size);

		m_buffer->Unlock(pbData, dwLength, pbData2, dwLength2);

		return S_OK;
	}

	return S_FALSE;
}

bool OutputDevice::IsPlaying()
{
	unsigned long dwStatus = 0;
	m_buffer->GetStatus(&dwStatus);

	if (dwStatus & DSBSTATUS_PLAYING)
			return true;

	return false;
}

HRESULT OutputDevice::Play()
{
	HRESULT hr;

	if (FAILED(hr = m_buffer->Play(0, 0, 0)))
		return hr;

	//Log("Chunk played");

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

void OutputDevice::StartPlaying()
{
	while (true)
	{
		if (m_playingAllowed)
		{
			if (!IsPlaying())
			{
				if (m_bufferFilled)
				{
					if (FAILED(Play()))
					{
						Log("buffer playing error");
						delete(m_currentData);
						return;
					}
				}
			}
			else
			{
				unsigned long rdPos;
				unsigned long wrPos;

				if (FAILED(m_buffer->GetCurrentPosition(&rdPos, &wrPos)))
					continue;

				if (rdPos == m_bufferSize / 2)
				{
					OnEvent(EVENT_FIRST_HALF_BUFFER_PLAYED);
				}

				if (rdPos == 0)
				{
					OnEvent(EVENT_SECOND_HALF_BUFFER_PLAYED);
				}
			}
		}
	}
}

void OutputDevice::HandleEvent()
{
	switch (event)
	{
	case EVENT_NEW_DATA_RECEIVED:
		HandleNewDataReceived();
		break;
	case EVENT_FIRST_HALF_BUFFER_PLAYED:
		HandleFirstHalfBufferPlayed();
		break;
	case EVENT_SECOND_HALF_BUFFER_PLAYED:
		HandleSecondHalfBufferPlayed();
		break;
	}
}

void OutputDevice::HandleNewDataReceived()
{
	ApplyNewData();

	if (!m_bufferFilled)
	{
		FillBuffer(0, m_bufferSize);

		m_bufferFilled = true;

		delete(m_currentData);
		m_currentData = nullptr;

		if (output)
		{
			output->OnEvent(EVENT_NEW_DATA_REQUESTED);
		}
	}
}

void OutputDevice::HandleFirstHalfBufferPlayed()
{
	if (!IsPlaying())
		return;

	FillBuffer(0, m_bufferSize / 2);
}

void OutputDevice::HandleSecondHalfBufferPlayed()
{
	if (!IsPlaying())
		return;

	FillBuffer(m_bufferSize / 2, m_bufferSize / 2);

	delete(m_currentData);
	m_currentData = nullptr;

	if (output)
	{
		output->OnEvent(EVENT_NEW_DATA_REQUESTED);
	}
}