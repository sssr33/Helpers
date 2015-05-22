#pragma once
#include "HSystemCommon.h"

#include <unordered_map>
#include <wincodec.h>

typedef GUID GUID_ContainerFormat;
typedef GUID GUID_WICPixelFormat;

class ImageUtilsStaticData{
protected:
	static std::unordered_map<GUID_WICPixelFormat, uint32_t, GUIDHash> WICPixelFormatBitSize;
private:
	static int StaticCtorTmp;
	static int StaticCtor();
};