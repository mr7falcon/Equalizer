#include "OutputDevice.h"

OutputDevice::OutputDevice()
	:m_pDS(NULL),
	m_bufferSize(defaultChunkSize * 2 * sectionCount),
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
	for (unsigned short i = 0; i < sectionCount; ++i)
	{
		dspn[i].dwOffset = (i + 1) * sectionSize - 1;
		dspn[i].hEventNotify = bufferEvents[i];
	}

	if (FAILED(hr = pDSNotify->SetNotificationPositions(sectionCount, dspn)))
	{
		return hr;
	}

	return S_OK;
}

HRESULT OutputDevice::FillBuffer(const unsigned long startPos, const byte* data)
{
	if (m_buffer)
	{
		HRESULT hr;

		void* pbData = nullptr;
		void* pbData2 = nullptr;
		unsigned long dwLength;
		unsigned long dwLength2;

		const unsigned long size = defaultChunkSize * 2;

		if (FAILED(hr = m_buffer->Lock(startPos, size, &pbData, &dwLength,
			&pbData2, &dwLength2, 0L)))
			return hr;

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
	std::unique_lock<std::mutex> locker(g_dataLock);

	const byte* encodedData = EncodeChunk(m_currentData);

	delete(m_currentData);
	m_currentData = nullptr;

	g_dataProcessed.notify_one();

	m_swapBuffer.WriteSwapSection(encodedData);

	if (!m_playingAllowed)
	{
		HandleSectionPlayed();

		if (m_currentSection == 0 && m_swapBuffer.GetWritePos() == 0)
		{
			m_playingAllowed = true;
		}
	}
}

void OutputDevice::HandleSectionPlayed()
{
	const byte* swapData = m_swapBuffer.ReadSwapSection();

	if (!swapData)
	{
		m_playingAllowed = false;
		return;
	}

	FillBufferSection(swapData);

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
		std::string str = std::to_string(i);
		bufferEvents[i] = CreateEvent(NULL, true, false, LPWSTR(str.c_str()));
	}
}

byte* OutputDevice::EncodeChunk(const DataChunk* newChunk)
{
	byte* data = new byte[newChunk->size * 2];

	int j = 0;
	for (unsigned long i = 0; i < newChunk->size; ++i)
	{
		data[j] = (byte)(newChunk->data[i]);
		data[++j] = (byte)(newChunk->data[i] >> 8);
		++j;
	}

	return data;
}

HRESULT OutputDevice::FillBufferSection(const byte* data)
{
	const unsigned long sectionSize = m_bufferSize / sectionCount;

	if (FAILED(FillBuffer(sectionSize * m_currentSection, data)))
	{
		return S_FALSE;
	}

	delete[](data);

	m_currentSection = ++m_currentSection % sectionCount;

	return S_OK;
}

SwapBuffer::SwapBuffer()
	:rdpos(0),
	wtpos(0)
{
	swapData = new const byte*[sectionCount];
}

SwapBuffer::~SwapBuffer()
{
	delete[](swapData);
}

const byte* SwapBuffer::ReadSwapSection()
{
	const byte* data = swapData[rdpos];
	rdpos = ++rdpos % sectionCount;

	return data;
}

void SwapBuffer::WriteSwapSection(const byte* data)
{
	swapData[wtpos] = data;
	wtpos = ++wtpos % sectionCount;
}