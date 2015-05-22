#pragma once
#include "HDataCommon.h"

#include <cstdint>
#include <functional>
#include <Windows.h>

struct GUIDHash{
	std::size_t operator()(const GUID &v) const{
		const uint64_t *tmpData4 = reinterpret_cast<const uint64_t *>(v.Data4);

		std::size_t h1 = std::hash<unsigned long>()(v.Data1);
		std::size_t h2 = std::hash<unsigned short>()(v.Data2);
		std::size_t h3 = std::hash<unsigned short>()(v.Data3);
		std::size_t h4 = std::hash<uint64_t>()(*tmpData4);

		return HDataCommon::CombineHash(h1, HDataCommon::CombineHash(h2, HDataCommon::CombineHash(h3, h4)));
	}
};