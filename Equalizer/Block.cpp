#include "Block.h"

std::mutex g_lock;

void Log(const char* str)
{
	g_lock.lock();
	std::cout << str << std::endl;
	g_lock.unlock();
}

void HandleData(Block* block, DataChunk* data)
{
	std::thread thd([=]()
	{
		if (block)
		{
			block->OnInput(data);
		}
	});

	thd.detach();
}

Block::Block(Block* output)
	:output(output)
{
}

Block::~Block()
{
}

void Block::SendData(DataChunk* data) const
{
	Log("Sending new data chunk");

	HandleData(output, data);
}

void Block::OnInput(DataChunk* data)
{
	HandleDataInternal(data);
}