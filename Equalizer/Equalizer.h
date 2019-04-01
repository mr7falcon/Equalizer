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

class Equalizer
{
public:
	Equalizer();
	~Equalizer();

	void Run(const char* fileName);
	void StartPlaying();
	void ShutDown();

	void ResetGainBands(const std::vector<int>& bandNums, Gains type);

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