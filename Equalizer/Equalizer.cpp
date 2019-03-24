#include <string>
#include "InputDevice.h"
#include "OutputDevice.h"
#include "FIR.h"
#include "Summator.h"

#pragma comment(lib, "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\\dsound.lib")
#pragma comment(lib, "C:\\Users\\mrfal\\source\\repos\\Equalizer\\Equalizer\\fftw\\libfftw3-3.lib")

int main(int argc, char** argv)
{
	HWND hDlg = GetForegroundWindow();

	InputDevice inputDevice;
	OutputDevice outputDevice;
	FIR filter0(0, 10);
	FIR filter1(1, 10);
	FIR filter2(2, 10);
	FIR filter3(3, 10);
	FIR filter4(4, 10);
	FIR filter5(5, 10);
	FIR filter6(6, 10);
	FIR filter7(7, 10);
	FIR filter8(8, 10);
	FIR filter9(9, 10);
	Summator sum(10);
	inputDevice.SetOutput(&filter0);
	inputDevice.SetOutput(&filter1);
	inputDevice.SetOutput(&filter2);
	inputDevice.SetOutput(&filter3);
	inputDevice.SetOutput(&filter4);
	inputDevice.SetOutput(&filter5);
	inputDevice.SetOutput(&filter6);
	inputDevice.SetOutput(&filter7);
	inputDevice.SetOutput(&filter8);
	inputDevice.SetOutput(&filter9);
	filter0.SetOutput(&sum);
	filter1.SetOutput(&sum);
	filter2.SetOutput(&sum);
	filter3.SetOutput(&sum);
	filter4.SetOutput(&sum);
	filter5.SetOutput(&sum);
	filter6.SetOutput(&sum);
	filter7.SetOutput(&sum);
	filter8.SetOutput(&sum);
	filter9.SetOutput(&sum);
	sum.SetOutput(&outputDevice);
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

	filter0.Init(waveFormat.wBitsPerSample);
	filter1.Init(waveFormat.wBitsPerSample);
	filter2.Init(waveFormat.wBitsPerSample);
	filter3.Init(waveFormat.wBitsPerSample);
	filter4.Init(waveFormat.wBitsPerSample);
	filter5.Init(waveFormat.wBitsPerSample);
	filter6.Init(waveFormat.wBitsPerSample);
	filter7.Init(waveFormat.wBitsPerSample);
	filter8.Init(waveFormat.wBitsPerSample);
	filter9.Init(waveFormat.wBitsPerSample);

	std::thread it([&]()
	{
		inputDevice.Run();
	});

	std::thread ft0([&]()
	{
		filter0.Run();
	});

	std::thread ft1([&]()
	{
		filter1.Run();
	});

	std::thread ft2([&]()
	{
		filter2.Run();
	});

	std::thread ft3([&]()
	{
		filter3.Run();
	});

	std::thread ft4([&]()
	{
		filter4.Run();
	});

	std::thread ft5([&]()
	{
		filter5.Run();
	});

	std::thread ft6([&]()
	{
		filter6.Run();
	});

	std::thread ft7([&]()
	{
		filter7.Run();
	});

	std::thread ft8([&]()
	{
		filter8.Run();
	});

	std::thread ft9([&]()
	{
		filter9.Run();
	});

	std::thread st([&]()
	{
		sum.Run();
	});

	std::thread ort([&]()
	{
		outputDevice.Run();
	});

	inputDevice.OnEvent(EVENT_NEW_DATA_REQUESTED);

	std::thread opt([&]()
	{
		outputDevice.StartPlaying();
	});

	while (true)
	{
		std::string command;
		if (std::cin >> command)
		{
			if (command == "up")
			{
				filter4.SetGain(-50);
				filter5.SetGain(-50);
				filter6.SetGain(-50);
				filter7.SetGain(-50);
				filter8.SetGain(-50);
				filter9.SetGain(-50);
				continue;
			}
			if (command == "rst")
			{
				filter4.SetGain(1);
				filter5.SetGain(1);
				filter6.SetGain(1);
				filter7.SetGain(1);
				filter8.SetGain(1);
				filter9.SetGain(1);
				continue;
			}
		}
	}

	it.join();
	ft0.join();
	ft1.join();
	ft2.join();
	ft3.join();
	ft4.join();
	ft5.join();
	ft6.join();
	ft7.join();
	ft8.join();
	ft9.join();
	st.join();
	ort.join();
	opt.join();
}