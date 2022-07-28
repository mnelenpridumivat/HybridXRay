#pragma once
#include "stdafx.h"

#include <fstream>
static void WriteLog(shared_str log)
{
	std::ofstream flog;
	flog.open("visual_log.log", std::ios::app); 

	flog << log.c_str() << std::endl;
	flog.close();
}