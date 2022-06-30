#include "stdafx.h"
#pragma hdrstop

#include "ResourceManager.h"

#ifndef REDITOR
#include "../../xrEngine/render.h"
#endif

#include "../../xrEngine/tntQAVI.h"

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
	pAVI				= NULL;
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
	if (pAVI)				bind		= fastdelegate::FastDelegate1<u32>(this,&CTexture::apply_avi);
	else if (!seqDATA.empty())	bind		= fastdelegate::FastDelegate1<u32>(this,&CTexture::apply_seq);
	else						bind		= fastdelegate::FastDelegate1<u32>(this,&CTexture::apply_normal);
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
	if (pAVI->NeedUpdate()){
		R_ASSERT(D3DRTYPE_TEXTURE == pSurface->GetType());
		ID3DTexture2D*	T2D		= (ID3DTexture2D*)pSurface;

		// AVI
		D3DLOCKED_RECT R;
		R_CHK	(T2D->LockRect(0,&R,NULL,0));
		R_ASSERT(R.Pitch == int(pAVI->m_dwWidth*4));
		//		R_ASSERT(pAVI->DecompressFrame((u32*)(R.pBits)));
		BYTE* ptr; pAVI->GetFrame(&ptr);
		CopyMemory(R.pBits,ptr,pAVI->m_dwWidth*pAVI->m_dwHeight*4);
		//		R_ASSERT(pAVI->GetFrame((BYTE*)(&R.pBits)));

		R_CHK	(T2D->UnlockRect(0));
	}
	CHK_DX(HW.pDevice->SetTexture(dwStage,pSurface));
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
	flags.bLoaded					= true;
	desc_cache						= 0;
	if (pSurface)					return;

	flags.bUser						= false;
	flags.MemoryUsage				= 0;
	if (0==stricmp(*cName,"$null"))	return;
	if (0!=strstr(*cName,"$user$"))	
	{
		flags.bUser	= true;
		return;
	}

	Preload							();
//#ifndef		DEDICATED_SERVER
#ifndef REDITOR
	if (!g_dedicated_server)
#endif
	{
		// Check for OGM
		string_path			fn;
#ifndef REDITOR
		if (FS.exist(fn,"$game_textures$",*cName,".ogm"))
		{
		} 
		else
#endif
			if (FS.exist(fn,"$game_textures$",*cName,".avi"))
		{
			// AVI
			pAVI = xr_new<CAviPlayerCustom>();

			if (!pAVI->Load(fn)) 
			{
				xr_delete(pAVI);
				FATAL				("Can't open video stream");
			} 
			else 
			{
				flags.MemoryUsage	= pAVI->m_dwWidth*pAVI->m_dwHeight*4;

				// Now create texture
				ID3DTexture2D*	pTexture = 0;
				HRESULT hrr = HW.pDevice->CreateTexture(
					pAVI->m_dwWidth,pAVI->m_dwHeight,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,
					&pTexture,NULL
					);
				pSurface	= pTexture;
				if (FAILED(hrr))
				{
					FATAL		("Invalid video stream");
					R_CHK		(hrr);
					xr_delete	(pAVI);
					pSurface = 0;
				}

			}
		} 
		else if (FS.exist(fn,"$game_textures$",*cName,".seq"))
		{
			// Sequence
			string256 buffer;
			IReader* _fs		= FS.r_open(fn);

			flags.seqCycles	= FALSE;
			_fs->r_string	(buffer,sizeof(buffer));
			if (0==stricmp	(buffer,"cycled"))
			{
				flags.seqCycles	= TRUE;
				_fs->r_string	(buffer,sizeof(buffer));
			}
			u32 fps	= atoi(buffer);
			seqMSPF		= 1000/fps;

			while (!_fs->eof())
			{
				_fs->r_string(buffer,sizeof(buffer));
				_Trim		(buffer);
				if (buffer[0])	
				{
					// Load another texture
					u32	mem  = 0;
					pSurface = ::RImplementation.texture_load	(buffer,mem);
					if (pSurface)	
					{
						// pSurface->SetPriority	(PRIORITY_LOW);
						seqDATA.push_back		(pSurface);
						flags.MemoryUsage		+= mem;
					}
				}
			}
			pSurface	= 0;
			FS.r_close	(_fs);
		} 
		else
		{
			// Normal texture
			u32	mem  = 0;
			pSurface = ::RImplementation.texture_load	(*cName,mem);

			// Calc memory usage and preload into vid-mem
			if (pSurface) {
				// pSurface->SetPriority	(PRIORITY_NORMAL);
				flags.MemoryUsage		=	mem;
			}
		}
//#endif
	}
	PostLoad	()		;
}

void CTexture::Unload	()
{
#ifdef DEBUG
	string_path				msg_buff;
	xr_sprintf				(msg_buff,sizeof(msg_buff),"* Unloading texture [%s] pSurface RefCount=",cName.c_str());
#endif // DEBUG

//.	if (flags.bLoaded)		Msg		("* Unloaded: %s",cName.c_str());
	
	flags.bLoaded			= FALSE;
	if (!seqDATA.empty())	{
		for (u32 I=0; I<seqDATA.size(); I++)
		{
			_RELEASE	(seqDATA[I]);
		}
		seqDATA.clear();
		pSurface	= 0;
	}
	flags.MemoryUsage = 0;

#ifdef DEBUG
	_SHOW_REF		(msg_buff, pSurface);
#endif // DEBUG

	_RELEASE								(pSurface);

	xr_delete		(pAVI);

	bind			= fastdelegate::FastDelegate1<u32>(this,&CTexture::apply_load);
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
