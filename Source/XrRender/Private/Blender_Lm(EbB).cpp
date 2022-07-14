// BlenderDefault.cpp: implementation of the CBlender_LmEbB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#include "blender_Lm(EbB).h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlender_LmEbB::CBlender_LmEbB	()
{
	description.CLS		= B_LmEbB;
	description.version	= 0x1;
	xr_strcpy				(oT2_Name,	"$null");
	xr_strcpy				(oT2_xform,	"$null");
}

CBlender_LmEbB::~CBlender_LmEbB	()
{
}

void	CBlender_LmEbB::Save(	IWriter& fs )
{
}

void	CBlender_LmEbB::Load(	IReader& fs, u16 version )
{
}

#if RENDER==R_R1
//////////////////////////////////////////////////////////////////////////
// R1
//////////////////////////////////////////////////////////////////////////
void	CBlender_LmEbB::Compile(CBlender_Compile& C)
{
}
#elif RENDER==R_R2
//////////////////////////////////////////////////////////////////////////
// R2
//////////////////////////////////////////////////////////////////////////
void	CBlender_LmEbB::Compile(CBlender_Compile& C)
{
	if (oBlend.value)	C.r_Pass	("lmapE","lmapE",TRUE,TRUE,FALSE,TRUE,D3DBLEND_SRCALPHA,	D3DBLEND_INVSRCALPHA,	TRUE,0);
	else				C.r_Pass	("lmapE","lmapE",TRUE);
	C.r_Sampler			("s_base",	C.L_textures[0]	);
	C.r_Sampler			("s_lmap",	C.L_textures[1]	);
	C.r_Sampler_clf		("s_hemi",	*C.L_textures[2]);
	C.r_Sampler			("s_env",	oT2_Name,false,D3DTADDRESS_CLAMP);
	C.r_End				();
}
#else
//////////////////////////////////////////////////////////////////////////
// R3
//////////////////////////////////////////////////////////////////////////
void	CBlender_LmEbB::Compile(CBlender_Compile& C)
{
	if (oBlend.value)	C.r_Pass	("lmapE","lmapE",TRUE,TRUE,FALSE,TRUE,D3DBLEND_SRCALPHA,	D3DBLEND_INVSRCALPHA,	TRUE,0);
	else				C.r_Pass	("lmapE","lmapE",TRUE);
	//C.r_Sampler			("s_base",	C.L_textures[0]	);
	C.r_dx10Texture			("s_base",	C.L_textures[0]	);
	C.r_dx10Sampler			("smp_base");
	//C.r_Sampler			("s_lmap",	C.L_textures[1]	);
	C.r_dx10Texture			("s_lmap",	C.L_textures[1]	);
	C.r_dx10Sampler			("smp_linear");
	//C.r_Sampler_clf		("s_hemi",	*C.L_textures[2]);
	C.r_dx10Texture			("s_hemi",	*C.L_textures[2]);
	C.r_dx10Sampler			("smp_rtlinear");
	//C.r_Sampler			("s_env",	oT2_Name,false,D3DTADDRESS_CLAMP);
	C.r_dx10Texture			("s_env",	oT2_Name);
	//C.r_dx10Sampler			("smp_rtlinear");
	C.r_End				();
}
#endif