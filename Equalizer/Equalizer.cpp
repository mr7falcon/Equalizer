#include "Equalizer.h"

Equalizer::Equalizer()
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
		IIR* filter = new IIR(i, numOfBands);
		filters.push_back(filter);
		blocks.push_back(filter);
		inputDevice->SetOutput(filter);
		filter->SetOutput(summator);
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
	std::thread* thrd = new std::thread([&]()
	{
		block->Run();
	});

	threads.push_back(thrd);
}

void Equalizer::Run(const char* fileName)
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

	for (unsigned short i = 0; i < blocks.size(); ++i)
	{
		RunBlock(blocks[i]);
	}

	inputDevice->OnEvent(EVENT_NEW_DATA_REQUESTED);
	StartPlaying();

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

void Equalizer::ResetGainBands(const std::vector<int>& bandNums, Gains type)
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