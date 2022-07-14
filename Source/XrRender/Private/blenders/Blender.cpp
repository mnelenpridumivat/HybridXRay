// Blender.cpp: implementation of the IBlender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#define _USE_32BIT_TIME_T
#include <time.h>

#include "Blender.h"

void CBlender_DESC::Setup	(LPCSTR N)
{
	// Name
	VERIFY(xr_strlen(N)<128);
	VERIFY(0==strchr(N,'.'));
	xr_strcpy(cName,N);
	strlwr(cName);
	
	xr_strcpy(cComputer,Core.CompName);			// Computer
	_tzset(); _time32( (__time32_t*)&cTime );	// Time
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IBlender::IBlender()
{
	xr_strcpy			(oT_Name,	"$base0");
	xr_strcpy			(oT_xform,	"$null");
}

IBlender::~IBlender()
{

}

void	IBlender::Save(IWriter& fs )
{
}

void	IBlender::Load(	IReader& fs, u16  )
{
}

void	IBlender::Compile(CBlender_Compile& C)
{
}
