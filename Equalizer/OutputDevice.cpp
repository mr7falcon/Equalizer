#include "OutputDevice.h"

OutputDevice::OutputDevice()
	:m_pDS(NULL),
	m_bufferSize(defaultChunkSize),
	m_buffer(nullptr),
	m_playingAllowed(false),
	m_bufferFilled(false),
	bufferEvents(nullptr)
{
	InitBufferEvents();
}

OutputDevice::~OutputDevice()
{
	delete[](bufferEvents);
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

	IDirectSoundNotify* pDSNotify;
	if (FAILED(hr = m_buffer->QueryInterface(IID_IDirectSoundNotify,
		(void**)&pDSNotify)))
	{
		return hr;
	}
		
	DSBPOSITIONNOTIFY dspn[bufferEventCount];
	dspn[EVENT_FIRST_HALF_BUFFER_PLAYED].dwOffset = m_bufferSize / 2 - 1;
	dspn[EVENT_FIRST_HALF_BUFFER_PLAYED].hEventNotify = bufferEvents[EVENT_FIRST_HALF_BUFFER_PLAYED];
	dspn[EVENT_SECOND_HALF_BUFFER_PLAYED].dwOffset = m_bufferSize - 1;
	dspn[EVENT_SECOND_HALF_BUFFER_PLAYED].hEventNotify = bufferEvents[EVENT_SECOND_HALF_BUFFER_PLAYED];

	if (FAILED(hr = pDSNotify->SetNotificationPositions(bufferEventCount, dspn)))
	{
		return hr;
	}

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

		byte* data = m_currentData->data + startPos;
		memcpy(pbData, data, size);

		if (FAILED(hr = m_buffer->Unlock(pbData, dwLength, pbData2, dwLength2)))
			return hr;

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

	if (FAILED(hr = m_buffer->Play(0, 0, DSBPLAY_LOOPING)))
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
				unsigned long retVal, eventNum;

				retVal = WaitForMultipleObjects(bufferEventCount, bufferEvents, FALSE, INFINITE);
				{
					if (retVal != WAIT_FAILED)
					{
						eventNum = retVal - WAIT_OBJECT_0;
						OnEvent(Events(eventNum));
						ResetEvent(bufferEvents[eventNum]);
					}
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

void OutputDevice::InitBufferEvents()
{
	bufferEvents = new HANDLE[2];
	bufferEvents[EVENT_FIRST_HALF_BUFFER_PLAYED] = CreateEvent(NULL, true, false, LPWSTR("FirstHalfPlayed"));
	bufferEvents[EVENT_SECOND_HALF_BUFFER_PLAYED] = CreateEvent(NULL, true, false, LPWSTR("SecondHalfPlayed"));
}