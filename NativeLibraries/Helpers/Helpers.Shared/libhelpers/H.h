#pragma once
#include "HText.h"
#include "HData.h"
#include "HSystem.h"
#include "HTime.h"
#include "HMath.h"

#include "WinRT/HDataWinRT.h"

#define WSTR1(x) L#x
#define WSTR2(x) WSTR1(x)
#define WSCONCAT(a, b) WSTR2(a) WSTR2(b)
#define ARRAY_SIZE(A) sizeof A / sizeof A[0]

class H{
public:
	typedef HText Text;
	typedef HData Data;
	typedef HSystem System;
	typedef HTime Time;
	typedef HMath Math;

#if HAVE_WINRT == 1
	class WinRT{
	public:
		typedef HDataWinRT Data;
	};
#endif
};