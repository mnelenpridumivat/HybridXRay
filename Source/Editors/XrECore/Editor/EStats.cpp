#include "stdafx.h"
#pragma hdrstop

#include "EStats.h"
#include "hw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEStats::CEStats()
{
	fFPS		= 30.f;
	fRFPS		= 30.f;
	fTPS		= 0;
	dwLevelSelFaceCount	= 0;
	dwLevelSelVertexCount=0;
}

CEStats::~CEStats()
{

}

void CEStats::Show(CGameFont* font)
{
}
