#pragma once

#include <thread>
#include <vector>
#include "InputDevice.h"
#include "OutputDevice.h"
#include "FIR.h"
#include "IIR.h"
#include "Summator.h"

enum Gains : unsigned short
{
	UP,
	DOWN,
	RESET
};

enum Filters : short
{
	FILTER_FIR,
	FILTER_IIR,

	FILTER_WRONG = -1
};

class Equalizer
{
public:
	Equalizer(const Filters filterType);
	~Equalizer();

	void Open(const char* fileName);
	void Run();
	void StartPlaying();
	void ShutDown();

	void SetGain(const unsigned short bandNum, const double gain);
	double GetGain(const unsigned short bandNum) const;
	void ResetGainBands(const std::vector<unsigned short>& bandNums, Gains type);

	void SetEffect(const Effects effect, bool reset = false) { summator->SetEffect(effect, reset); }
	void SetClippingCoef(const short coef) { summator->SetClippingCoef(coef); }

	void PorP();

private:
	std::vector<Block*> blocks;
	std::vector<std::thread*> threads;

	const unsigned short numOfBands;
	std::vector<Filter*> filters;
	InputDevice* inputDevice;
	OutputDevice* outputDevice;
	Summator* summator;

	void RunBlock(Block* block);
};