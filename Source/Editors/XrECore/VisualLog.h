#pragma once
#include "stdafx.h"

#include <fstream>
static void WriteLog(const char *format, ...)
{
	va_list		mark;
	string2048	buf;
	va_start	(mark, format );
	int sz		= _vsnprintf(buf, sizeof(buf)-1, format, mark ); buf[sizeof(buf)-1]=0;
	va_end		(mark);

	std::ofstream flog;
	flog.open("visual_log.log", std::ios::app); 

	flog << buf << std::endl;
	flog.close();
}