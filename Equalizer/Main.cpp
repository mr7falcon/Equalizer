#include <string>
#include "Equalizer.h"

#pragma comment(lib, "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\\dsound.lib")
#pragma comment(lib, "C:\\Users\\mrfal\\source\\repos\\Equalizer\\Equalizer\\fftw\\libfftw3-3.lib")

const std::vector<int> ExtractBandNums()
{
	std::string bandNums;
	std::cin >> bandNums;
	std::vector<int> bandNumsVec;

	if (bandNums == "all")
	{
		for (int i = 0; i < 10; ++i)
		{
			bandNumsVec.push_back(i);
		}
	}
	else
	{
		for (int i = 0; i < bandNums.length(); ++i)
		{
			bandNumsVec.push_back(atoi(bandNums.substr(i, 1).c_str()));
		}
	}

	return bandNumsVec;
}

int main(int argc, char** argv)
{
	Equalizer equalizer;

	std::thread commandThread([&]()
	{
		std::string command;
		while (std::cin >> command)
		{
			if (command == "up")
			{
				equalizer.ResetGainBands(ExtractBandNums(), Gains::UP);
				continue;
			}

			if (command == "down")
			{
				equalizer.ResetGainBands(ExtractBandNums(), Gains::DOWN);
				continue;
			}

			if (command == "reset")
			{
				equalizer.ResetGainBands(ExtractBandNums(), Gains::RESET);
				continue;
			}
		}
	});
	commandThread.detach();
	
	equalizer.Run(argv[1]);

	return 0;
}