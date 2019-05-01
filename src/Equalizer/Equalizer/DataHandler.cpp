#include "DataHandler.h"

DataHandler::DataHandler()
	:output(nullptr)
{
}

DataHandler::~DataHandler()
{
}

void DataHandler::SendNewData(const short* newCurrentData)
{
	std::unique_lock<std::mutex> locker(dataLock);
	while (IsProcessingData())
	{
		dataProcessed.wait(locker);
	}

	currentData = newCurrentData;
	OnEvent(EVENT_NEW_DATA_RECEIVED);
}