#pragma once

#include <cstdint>

class HDataCommon{
public:
	static void HashRound(uint32_t &hash, uint32_t v){
		// http://stackoverflow.com/questions/2285822/c-what-is-a-good-way-to-hash-array-data
		hash = (hash + (324723947 + v)) ^ (uint32_t)93485734985;
	}

	static size_t CombineHash(size_t a, size_t b){
		// based on http://en.cppreference.com/w/cpp/utility/hash
		return a ^ (b << 1);
	}
};