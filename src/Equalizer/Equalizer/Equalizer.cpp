#include "Equalizer.h"

#pragma comment(lib, "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\\dsound.lib")

Equalizer::Equalizer(const Filters filterType)
	:numOfBands(10)
{
	inputDevice = new InputDevice();
	blocks.push_back(inputDevice);

	outputDevice = new OutputDevice();
	blocks.push_back(outputDevice);
	outputDevice->SetOutput(inputDevice);

	summator = new Summator(numOfBands);
	blocks.push_back(summator);
	summator->SetOutput(outputDevice);

	for (unsigned short i = 0; i < numOfBands; ++i)
	{
		Filter* filter = nullptr;

		switch (filterType)
		{
		case FILTER_FIR:
			filter = new FIR(i, numOfBands);
			break;
		case FILTER_IIR:
			filter = new IIR(i, numOfBands);
		}
		
		if (filter)
		{
			filters.push_back(filter);
			blocks.push_back(filter);
			inputDevice->SetOutput(filter);
			filter->SetOutput(summator);
		}
	}
}

Equalizer::~Equalizer()
{
	for (unsigned short i = 0; i < blocks.size(); ++i)
	{
		delete(blocks[i]);
	}

	for (unsigned short i = 0; i < threads.size(); ++i)
	{
		delete(threads[i]);
	}
}

void Equalizer::RunBlock(Block* block)
{
	std::thread* thrd = new std::thread([=]()
	{
		block->Run();
	});

	threads.push_back(thrd);
}

void Equalizer::Open(const char* fileName)
{
	HWND hDlg = GetForegroundWindow();

	WAVEFORMATEX waveFormat;
	if (!inputDevice->OpenFile(fileName, waveFormat))
	{
		return;
	}

	if (FAILED(outputDevice->Init(hDlg, waveFormat)))
	{
		return;
	}
}

void Equalizer::Run()
{
	for (unsigned short i = 0; i < blocks.size(); ++i)
	{
		RunBlock(blocks[i]);
	}

	inputDevice->OnEvent(EVENT_NEW_DATA_REQUESTED);

	for (unsigned short i = 0; i < threads.size(); ++i)
	{
		threads[i]->join();
	}
}

void Equalizer::StartPlaying()
{
	std::thread* opt = new std::thread([&]()
 	{
 		outputDevice->StartPlaying();
 	});

	opt->detach();
}

void Equalizer::SetGain(const unsigned short bandNum, const double gain)
{
	filters[bandNum]->SetGain(gain);
}

double Equalizer::GetGain(const unsigned short bandNum) const
{
	return filters[bandNum]->GetGain();
}

void Equalizer::ResetGainBands(const std::vector<unsigned short>& bandNums, Gains type)
{
	float mult = 0;
	switch (type)
	{
	case UP:
		mult = 5;
		break;
	case DOWN:
		mult = -50;
		break;
	case RESET:
		mult = 0;
		break;
	default:
		return;
	}

	for (int i = 0; i < bandNums.size(); ++i)
	{
		filters[bandNums[i]]->SetGain(mult);
	}
}

void Equalizer::ShutDown()
{
	inputDevice->OnEvent(EVENT_SHUTDOWN);
	for (unsigned short i = 0; i < numOfBands; ++i)
	{
		filters[i]->OnEvent(EVENT_SHUTDOWN);
	}
	summator->OnEvent(EVENT_SHUTDOWN);
	outputDevice->OnEvent(EVENT_SHUTDOWN);
}

void Equalizer::PorP()
{
	HRESULT hr;

	if (outputDevice->IsPlaying())
	{
		hr = outputDevice->Stop();
	}
	else
	{
		hr = outputDevice->Play();
	}

	if (FAILED(hr))
		Log("failed to play or pause");
}