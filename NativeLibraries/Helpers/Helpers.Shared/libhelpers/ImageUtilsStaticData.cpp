#if INCLUDE_PCH_H == 1
#include "pch.h"
#endif
#include "ImageUtilsStaticData.h"

std::unordered_map<GUID_WICPixelFormat, uint32_t, GUIDHash> ImageUtilsStaticData::WICPixelFormatBitSize;
int ImageUtilsStaticData::StaticCtorTmp = ImageUtilsStaticData::StaticCtor();

int ImageUtilsStaticData::StaticCtor(){

	// TODO: add bit sizes for all formats
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat32bppPBGRA, 32));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat32bppPRGBA, 32));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat32bppBGRA, 32));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat32bppRGBA, 32));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat24bppBGR, 24));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat24bppRGB, 24));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat1bppIndexed, 1));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat2bppIndexed, 2));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat4bppIndexed, 4));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat8bppIndexed, 8));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormatBlackWhite, 1));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat2bppGray, 2));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat4bppGray, 4));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat8bppGray, 8));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat8bppAlpha, 8));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat16bppBGR555, 16));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat16bppBGR565, 16));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat16bppBGRA5551, 16));
	WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat16bppGray, 16));

	return 0;
}