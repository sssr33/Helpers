// Helpers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "containers\chunked_data_buffer.h"

#include <queue>

int _tmain(int argc, _TCHAR* argv[])
{
	size_t sz = 0;
	uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7 };
	chunked_data_buffer<uint8_t, 3> buffer;

	buffer.push_back(1);
	sz = buffer.size();

	buffer.pop_front(1);
	sz = buffer.size();

	buffer.push_back(1);
	sz = buffer.size();

	buffer.pop_front(1);
	sz = buffer.size();

	buffer.push_back(data, data + sizeof(data));
	sz = buffer.size();

	buffer.pop_front();
	sz = buffer.size();

	buffer.pop_front();
	sz = buffer.size();

	buffer.pop_front(2);
	sz = buffer.size();

	buffer.pop_front(3);
	sz = buffer.size();

	return 0;
}

