#include <windows.h>
#include <thread>
#include "InputDevice.h"
#include "OutputDevice.h"
#include "WaveFormat.h"

#pragma comment(lib, "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\\dsound.lib")

int main(int argc, char** argv)
{
	HWND hDlg = GetForegroundWindow();

	OutputDevice outputDevice;
	InputDevice inputDevice(&outputDevice);
	outputDevice.SetInputDevice(&inputDevice);

	WAVEFORMATEX waveFormat;
	if (!inputDevice.OpenFile(argv[1], waveFormat))
	{
		return 1;
	}

	if (FAILED(outputDevice.Init(hDlg, waveFormat)))
	{
		return 1;
	}

	for (int i = 0; i < buffersCount; ++i)
	{
		RequestNewDataChunk(&inputDevice);
	}

	Sleep(40000);
}