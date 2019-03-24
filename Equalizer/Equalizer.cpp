#include "Equalizer.h"

Equalizer::Equalizer()
	:numOfBands(10)
{
	inputDevice = new InputDevice();
	blocks.push_back(inputDevice);
	inputDevice->SetOutput(outputDevice);

	outputDevice = new OutputDevice();
	blocks.push_back(outputDevice);
	outputDevice->SetOutput(inputDevice);

	summator = new Summator(numOfBands);
	blocks.push_back(summator);
	summator->SetOutput(outputDevice);

	for (unsigned short i = 0; i < numOfBands; ++i)
	{
		FIR* filter = new FIR(i, numOfBands);
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

	for (unsigned short i = 0; i < threads.size(); ++i)
	{
		threads[i]->join();
	}
}

void Equalizer::StartPlaying()
{
	std::thread opt([&]()
 	{
 		outputDevice->StartPlaying();
 	});

	opt.detach();
}

void Equalizer::ResetGainBands(const std::vector<int>& bandNums, Gains type)
{
	float mult = 0;
	switch (type)
	{
	case UP:
		mult = 2.f;
		break;
	case DOWN:
		mult = -50.f;
		break;
	case RESET:
		mult = 1.f;
		break;
	default:
		return;
	}

	for (int i = 0; i < bandNums.size(); ++i)
	{
		filters[bandNums[i]]->SetGain(mult);
	}
}