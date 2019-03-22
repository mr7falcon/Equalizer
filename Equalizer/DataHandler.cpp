#include "DataHandler.h"

std::mutex g_lock;
std::condition_variable g_dataProcessed;

DataHandler::DataHandler()
	:output(nullptr)
{
}

DataHandler::~DataHandler()
{
}

void DataHandler::SendNewData(DataChunk* newCurrentData)
{
	std::unique_lock<std::mutex> locker(g_lock);

	m_currentData = newCurrentData;
	OnEvent(EVENT_NEW_DATA_RECEIVED);

	g_dataProcessed.wait(locker);
}