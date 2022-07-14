#include "stdafx.h"
#pragma hdrstop

#include "Blender_Screen_SET.h"

#define		VER_2_oBlendCount	7
#define		VER_4_oBlendCount	9
#define		VER_5_oBlendCount	10

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlender_Screen_SET::CBlender_Screen_SET()
{
}

CBlender_Screen_SET::~CBlender_Screen_SET()
{
	
}

void	CBlender_Screen_SET::Save	( IWriter& fs	)
{
}

void	CBlender_Screen_SET::Load	( IReader& fs, u16 version)
{
}

#if defined(USE_DX10) || defined(USE_DX11)

void	CBlender_Screen_SET::Compile	(CBlender_Compile& C)
{
	IBlender::Compile		(C);
	//C.r_Pass			("stub_notransform_t", "Blender_Screen_SET", false);

	if (oBlend.IDselected==6)
	{
		// Usually for wallmarks
		C.r_Pass			("stub_notransform_t", "stub_default_ma", false);

		VERIFY(C.L_textures.size()>0);
		C.r_dx10Texture			("s_base",	C.L_textures[0]	);
		int iSmp = C.r_dx10Sampler("smp_base");
		if (oClamp.value)
			C.i_dx10Address(iSmp, D3DTADDRESS_CLAMP);
		
	} 
	else 
	{
		if (9==oBlend.IDselected)
		{
			// 4x R
			C.r_Pass			("stub_notransform_t_m4", "stub_default", false);
			//C.StageSET_Color	(D3DTA_TEXTURE,	  D3DTOP_MODULATE4X,	D3DTA_DIFFUSE);
			//C.StageSET_Alpha	(D3DTA_TEXTURE,	  D3DTOP_SELECTARG1,	D3DTA_DIFFUSE);
		} 
		else 
		{
			if ((7==oBlend.IDselected) || (8==oBlend.IDselected))
			{
				// 2x R
				C.r_Pass			("stub_notransform_t_m2", "stub_default", false);
				//C.StageSET_Color	(D3DTA_TEXTURE,	  D3DTOP_MODULATE2X,	D3DTA_DIFFUSE);
				//C.StageSET_Alpha	(D3DTA_TEXTURE,	  D3DTOP_SELECTARG1,	D3DTA_DIFFUSE);
			} 
			else 
			{
				// 1x R
				C.r_Pass			("stub_notransform_t", "stub_default", false);
				//C.StageSET_Color	(D3DTA_TEXTURE,	  D3DTOP_MODULATE,		D3DTA_DIFFUSE);
				//C.StageSET_Alpha	(D3DTA_TEXTURE,	  D3DTOP_MODULATE,		D3DTA_DIFFUSE);
			}
		}
		//C.Stage_Texture		(oT_Name);
		//C.Stage_Matrix		(oT_xform,	0);
		//C.Stage_Constant	("$null");
		//C.StageEnd			();
		VERIFY(C.L_textures.size()>0);
		C.r_dx10Texture			("s_base",	C.L_textures[0]	);
		int iSmp = C.r_dx10Sampler("smp_base");
		if ( (oClamp.value) && (iSmp != u32(-1)) )
			C.i_dx10Address(iSmp, D3DTADDRESS_CLAMP);
	}

	C.PassSET_ZB		(oZTest.value,oZWrite.value);

	switch (oBlend.IDselected)
	{
	case 0:	// SET
		C.PassSET_Blend	(FALSE,	D3DBLEND_ONE,D3DBLEND_ZERO,				FALSE,0);
		break;
	case 1: // BLEND
		C.PassSET_Blend	(TRUE,	D3DBLEND_SRCALPHA,D3DBLEND_INVSRCALPHA,	TRUE,oAREF.value);
		break;
	case 2:	// ADD
		C.PassSET_Blend	(TRUE,	D3DBLEND_ONE,D3DBLEND_ONE,				FALSE,oAREF.value);
		break;
	case 3:	// MUL
		C.PassSET_Blend	(TRUE,	D3DBLEND_DESTCOLOR,D3DBLEND_ZERO,		FALSE,oAREF.value);
		break;
	case 4:	// MUL_2X
		C.PassSET_Blend	(TRUE,	D3DBLEND_DESTCOLOR,D3DBLEND_SRCCOLOR,	FALSE,oAREF.value);
		break;
	case 5:	// ALPHA-ADD
		C.PassSET_Blend	(TRUE,	D3DBLEND_SRCALPHA,D3DBLEND_ONE,			TRUE,oAREF.value);
		break;
	case 6:	// MUL_2X + A-test
		C.PassSET_Blend	(TRUE,	D3DBLEND_DESTCOLOR,D3DBLEND_SRCCOLOR,	FALSE,oAREF.value);
		break;
	case 7:	// SET (2r)
		C.PassSET_Blend	(TRUE,	D3DBLEND_ONE,D3DBLEND_ZERO,				TRUE,0);
		break;
	case 8: // BLEND (2r)
		C.PassSET_Blend	(TRUE,	D3DBLEND_SRCALPHA,D3DBLEND_INVSRCALPHA,	TRUE,oAREF.value);
		break;
	case 9: // BLEND (2r)
		C.PassSET_Blend	(TRUE,	D3DBLEND_SRCALPHA,D3DBLEND_INVSRCALPHA,	TRUE,oAREF.value);
		break;
	}
	C.PassSET_LightFog	(oLighting.value,oFog.value);

	C.r_End				();
}

#else	//	USE_DX10

void	CBlender_Screen_SET::Compile	(CBlender_Compile& C)
{
}
#endif	//	USE_DX10