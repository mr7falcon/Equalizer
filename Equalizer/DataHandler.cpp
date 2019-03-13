#include "DataHandler.h"

DataHandler::DataHandler()
	:m_newData(nullptr)
{
}

DataHandler::~DataHandler()
{
}

void DataHandler::SendNewData(DataChunk* newCurrentData)
{
	g_lock.lock();

	m_newData = newCurrentData;
	OnEvent(EVENT_NEW_DATA_RECEIVED);

	g_lock.unlock();
}

void DataHandler::ApplyNewData()
{
	g_lock.lock();

	m_currentData = m_newData;
	m_newData = nullptr;

	g_lock.unlock();
}