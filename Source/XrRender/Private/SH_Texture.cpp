#include "stdafx.h"
#pragma hdrstop

#include "ResourceManager.h"

#ifndef REDITOR
#include "../../xrEngine/render.h"
#endif

#include "dxRenderDeviceRender.h"

#define		PRIORITY_HIGH	12
#define		PRIORITY_NORMAL	8
#define		PRIORITY_LOW	4



void resptrcode_texture::create(LPCSTR _name)
{
	_set(DEV->_CreateTexture(_name));
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTexture::CTexture		()
{
	pSurface			= NULL;
	desc_cache			= 0;
	seqMSPF				= 0;
	flags.MemoryUsage	= 0;
	flags.bLoaded		= false;
	flags.bUser			= false;
	flags.seqCycles		= FALSE;
	m_material			= 1.0f;
	bind				= fastdelegate::FastDelegate1<u32>(this,&CTexture::apply_load);
}

CTexture::~CTexture()
{
	Unload				();

	// release external reference
	DEV->_DeleteTexture	(this);
}

void					CTexture::surface_set	(ID3DBaseTexture* surf )
{
	if (surf)			surf->AddRef		();

	_RELEASE								(pSurface);

	pSurface			= surf;
}

ID3DBaseTexture*	CTexture::surface_get	()
{
	if (pSurface)		pSurface->AddRef	();
	return pSurface;
}

void CTexture::PostLoad	()
{
}

void CTexture::apply_load	(u32 dwStage)	{
	if (!flags.bLoaded)		Load			()	;
	else					PostLoad		()	;
	bind					(dwStage)			;
};

void CTexture::apply_theora	(u32 dwStage)
{
};
void CTexture::apply_avi	(u32 dwStage)	
{
};
void CTexture::apply_seq	(u32 dwStage)	{
	// SEQ
	u32	frame		=Device->dwTimeContinual/seqMSPF; //Device->dwTimeGlobal
	u32	frame_data	= seqDATA.size();
	if (flags.seqCycles)		{
		u32	frame_id	= frame%(frame_data*2);
		if (frame_id>=frame_data)	frame_id = (frame_data-1) - (frame_id%frame_data);
		pSurface 			= seqDATA[frame_id];
	} else {
		u32	frame_id	= frame%frame_data;
		pSurface 			= seqDATA[frame_id];
	}
	CHK_DX(HW.pDevice->SetTexture(dwStage,pSurface));
};
void CTexture::apply_normal	(u32 dwStage)	{
	CHK_DX(HW.pDevice->SetTexture(dwStage,pSurface));
};

void CTexture::Preload	()
{
	m_bumpmap = DEV->m_textures_description.GetBumpName(cName);
	m_material = DEV->m_textures_description.GetMaterial(cName);
}

void CTexture::Load		()
{
}

void CTexture::Unload	()
{
}

void CTexture::desc_update	()
{
	desc_cache	= pSurface;
	if (pSurface && (D3DRTYPE_TEXTURE == pSurface->GetType()))
	{
		ID3DTexture2D*	T	= (ID3DTexture2D*)pSurface;
		R_CHK					(T->GetLevelDesc(0,&desc));
	}
}

void CTexture::video_Play		(BOOL looped, u32 _time)	
{ 
}

void CTexture::video_Pause		(BOOL state)
{
}

void CTexture::video_Stop			()				
{ 
}

BOOL CTexture::video_IsPlaying	()				
{ 
	return FALSE;
}
