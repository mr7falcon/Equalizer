#include <string>
#include "InputDevice.h"
#include "OutputDevice.h"

#pragma comment(lib, "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\\dsound.lib")

int main(int argc, char** argv)
{
	HWND hDlg = GetForegroundWindow();

	InputDevice inputDevice;
	OutputDevice outputDevice;
	inputDevice.SetOutput(&outputDevice);
	outputDevice.SetOutput(&inputDevice);

	WAVEFORMATEX waveFormat;
	if (!inputDevice.OpenFile(argv[1], waveFormat))
	{
		return 1;
	}

	if (FAILED(outputDevice.Init(hDlg, waveFormat)))
	{
		return 1;
	}

	std::thread it([&]()
	{
		inputDevice.Run();
	});
	std::thread ort([&]()
	{
		outputDevice.Run();
	});

	for (int i = 0; i < buffersCount; ++i)
	{
		inputDevice.SendNewData(new DataChunk);
	}

	std::thread opt([&]()
	{
		outputDevice.StartPlaying();
	});

	while (true)
	{
		std::string command;
		if (std::cin >> command)
		{
			if (command == "play")
			{
				outputDevice.SetPlayingAllowed(true);
				continue;
			}

			if (command == "pause")
			{
				outputDevice.SetPlayingAllowed(false);
				continue;
			}
		}
	}

	it.join();
	ort.join();
	opt.join();
}