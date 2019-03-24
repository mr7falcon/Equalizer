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
	std::unique_lock<std::mutex> locker(g_dataLock);
	while (IsProcessingData())
	{
		g_dataProcessed.wait(locker);
	}

	m_currentData = newCurrentData;
	OnEvent(EVENT_NEW_DATA_RECEIVED);
}