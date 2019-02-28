#include "InputDevice.h"

InputDevice::InputDevice()
{

}

InputDevice::~InputDevice()
{

}

HRESULT InputDevice::Open(const char* strFilename)
{
	HRESULT hr;

	if (FAILED(hr = WaveOpenFile(strFilename)))
		return hr;

	if (FAILED(hr = Reset()))
		return hr;

	return hr;
}

HRESULT InputDevice::WaveOpenFile(const char* strFileName)
{
	HRESULT hr;

	if (NULL == (m_hmmioIn = mmioOpen(strFileName, NULL, MMIO_ALLOCBUF | MMIO_READ)))
		return E_FAIL;

	if (FAILED(hr = ReadMMIO()))
	{
		mmioClose(m_hmmioIn, 0);
		return hr;
	}

	return S_OK;
}

HRESULT InputDevice::ReadMMIO()
{

}