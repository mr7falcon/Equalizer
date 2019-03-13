#include "OutputDevice.h"

OutputDevice::OutputDevice()
	:m_pDS(NULL),
	m_bufferSize(defaultChunkSize * sectionCount),
	m_buffer(nullptr),
	m_playingAllowed(false),
	bufferEvents(nullptr),
	m_currentSection(0)
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
		
	DSBPOSITIONNOTIFY dspn[sectionCount];
	const unsigned long sectionSize = m_bufferSize / sectionCount;
	for (unsigned short i = sectionCount; i > 0; --i)
	{
		dspn[i - 1].dwOffset = i * sectionSize - 1;
		dspn[i - 1].hEventNotify = bufferEvents[i - 1];
	}

	if (FAILED(hr = pDSNotify->SetNotificationPositions(sectionCount, dspn)))
	{
		return hr;
	}

	return S_OK;
}

HRESULT OutputDevice::FillBuffer(const unsigned long startPos)
{
	if (m_buffer && m_currentData)
	{
		HRESULT hr;

		void* pbData = nullptr;
		void* pbData2 = nullptr;
		unsigned long dwLength;
		unsigned long dwLength2;

		const unsigned long size = m_currentData->size;

		if (FAILED(hr = m_buffer->Lock(startPos, size, &pbData, &dwLength,
			&pbData2, &dwLength2, 0L)))
			return hr;

		memcpy(pbData, m_currentData->data, size);

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
				if (FAILED(Play()))
				{
					Log("buffer playing error");
					delete(m_currentData);
					return;
				}
			}
			else
			{
				unsigned long retVal, eventNum;

				retVal = WaitForMultipleObjects(sectionCount, bufferEvents, FALSE, INFINITE);
				if (retVal != WAIT_FAILED)
				{
					eventNum = retVal - WAIT_OBJECT_0;
					OnEvent(EVENT_SECTION_PLAYED);
					ResetEvent(bufferEvents[eventNum]);
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
	case EVENT_SECTION_PLAYED:
		HandleSectionPlayed();
		break;
	}
}

void OutputDevice::HandleNewDataReceived()
{
	ApplyNewData();

	if (!IsPlaying() && !m_playingAllowed)
	{
		HandleSectionPlayed();

		if (m_currentSection == 0)
		{
			m_playingAllowed = true;
		}
	}
}

void OutputDevice::HandleSectionPlayed()
{
	if (!m_currentData)
	{
		m_playingAllowed = false;
		return;
	}

	const unsigned long sectionSize = m_bufferSize / sectionCount;

	FillBuffer((sectionSize) * m_currentSection);
	m_currentSection = ++m_currentSection % sectionCount;

	delete(m_currentData);
	m_currentData = nullptr;

	if (output)
	{
		output->OnEvent(EVENT_NEW_DATA_REQUESTED);
	}
}

void OutputDevice::InitBufferEvents()
{
	bufferEvents = new HANDLE[sectionCount];
	
	for (unsigned short i = 0; i < sectionCount; ++i)
	{
		bufferEvents[i] = CreateEvent(NULL, true, false, LPWSTR("PositionAchieved"));
	}
}