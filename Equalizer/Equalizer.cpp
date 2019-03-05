#include <windows.h>
#include "InputDevice.h"
#include "OutputDevice.h"
#include "WaveFormat.h"

#pragma comment(lib, "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\\dsound.lib")

int main(int argc, char** argv)
{
	HWND hDlg = GetForegroundWindow();

	InputDevice inputDevice;
	OutputDevice outputDevice;

	WAVEFORMATEX waveFormat;
	if (!inputDevice.OpenFile(argv[1], waveFormat))
	{
		return 1;
	}

	if (FAILED(outputDevice.Init(hDlg, waveFormat)))
	{
		return 1;
	}

	for (;;)
	{
		if (!outputDevice.IsBufferPlaying())
		{
			DataChunk* data = inputDevice.GetNextChunk();

			if (FAILED(outputDevice.PlayChunk(hDlg, data)))
			{
				return 1;
			}
		}
	}
}