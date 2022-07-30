#pragma once
#include "stdafx.h"

#include <iostream>

static void WriteLog(const char *format, ...)
{
	va_list		mark;
	string2048	buf;
	va_start	(mark, format );
	int sz		= _vsnprintf(buf, sizeof(buf)-1, format, mark ); buf[sizeof(buf)-1]=0;
	va_end		(mark);
	if (!Core.DebugLog)
		std::cout << buf << std::endl;
}