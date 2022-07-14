// HW.cpp: implementation of the CHW class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#pragma warning(disable:4995)
#include "directx\d3dx9.h"
#pragma warning(default:4995)
#include "HW.h"

#ifndef REDITOR
#include "../../xrEngine/XR_IOConsole.h"

	void	fill_vid_mode_list			(CHW* _hw);
	void	free_vid_mode_list			();

	void	fill_render_mode_list		();
	void	free_render_mode_list		();
#else
	void	fill_vid_mode_list			(CHW* _hw)	{}
	void	free_vid_mode_list			()			{}
	void	fill_render_mode_list		()			{}
	void	free_render_mode_list		()			{}
#endif

 CHW			HW;

#ifdef DEBUG
IDirect3DStateBlock9*	dwDebugSB = 0;
#endif

CHW::CHW() : 
	hD3D(NULL),
	pD3D(NULL),
	pDevice(NULL),
	pBaseRT(NULL),
	pBaseZB(NULL),
	m_move_window(true)
{
	;
}

CHW::~CHW()
{
	;
}

void CHW::Reset		(HWND hwnd)
{
#ifdef DEBUG
	_RELEASE			(dwDebugSB);
#endif
	_RELEASE			(pBaseZB);
	_RELEASE			(pBaseRT);

	while	(TRUE)	{
		HRESULT _hr							= HW.pDevice->Reset	(&DevPP);
		R_CHK(_hr);
		if (SUCCEEDED(_hr))					break;
	}
	R_CHK				(pDevice->GetRenderTarget			(0,&pBaseRT));
	R_CHK				(pDevice->GetDepthStencilSurface	(&pBaseZB));
#ifdef DEBUG
	R_CHK				(pDevice->CreateStateBlock			(D3DSBT_ALL,&dwDebugSB));
#endif
#ifndef REDITOR
	updateWindowProps	(hwnd);
#endif
}

//xr_token*				vid_mode_token = NULL;
//extern xr_token*		vid_mode_token;
#include "../../xrAPI/xrAPI.h"
//xr_token*				vid_quality_token = NULL;

void CHW::CreateD3D	()
{
//#ifndef DEDICATED_SERVER
//	LPCSTR		_name			= "d3d9.dll";
//#else
//	LPCSTR		_name			= "xrd3d9-null.dll";
//#endif

	LPCSTR		_name			= "xrd3d9-null.dll";

#ifndef REDITOR
	if (!g_dedicated_server)
#endif    
		_name			= "d3d9.dll";


	hD3D            			= LoadLibrary(_name);
	R_ASSERT2	           	 	(hD3D,"Can't find 'd3d9.dll'\nPlease install latest version of DirectX before running this program");
    typedef IDirect3D9 * WINAPI _Direct3DCreate9(UINT SDKVersion);
	_Direct3DCreate9* createD3D	= (_Direct3DCreate9*)GetProcAddress(hD3D,"Direct3DCreate9");	R_ASSERT(createD3D);
    this->pD3D 					= createD3D( D3D_SDK_VERSION );
    R_ASSERT2					(this->pD3D,"Please install DirectX 9.0c");
}

void CHW::DestroyD3D()
{
	_RELEASE					(this->pD3D);
    FreeLibrary					(hD3D);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
D3DFORMAT CHW::selectDepthStencil	(D3DFORMAT fTarget)
{
	// R2 hack
#pragma todo("R2 need to specify depth format")

	// R1 usual
	static	D3DFORMAT	fDS_Try1[6] =
	{D3DFMT_D24S8,D3DFMT_D24X4S4,D3DFMT_D32,D3DFMT_D24X8,D3DFMT_D16,D3DFMT_D15S1};

	D3DFORMAT*	fDS_Try			= fDS_Try1;
	int			fDS_Cnt			= 6;

	for (int it = 0; it<fDS_Cnt; it++){
		if (SUCCEEDED(pD3D->CheckDeviceFormat(
			DevAdapter,DevT,fTarget,
			D3DUSAGE_DEPTHSTENCIL,D3DRTYPE_SURFACE,fDS_Try[it])))
		{
            if( SUCCEEDED( pD3D->CheckDepthStencilMatch(
				DevAdapter,DevT,
                fTarget, fTarget, fDS_Try[it]) ) )
            {
				return fDS_Try[it];
            }
		}
	}
	return D3DFMT_UNKNOWN;
}

void	CHW::DestroyDevice	()
{
}

void		CHW::CreateDevice		(HWND m_hWnd, bool move_window)
{
}

u32	CHW::selectPresentInterval	()
{
	D3DCAPS9	caps;
	pD3D->GetDeviceCaps(DevAdapter,DevT,&caps);
	return D3DPRESENT_INTERVAL_DEFAULT;
}

u32 CHW::selectGPU ()
{
#if RENDER == R_R1
	BOOL isIntelGMA = FALSE;

	if ( Caps.id_vendor == 0x8086 ) { // Intel

		#define GMA_SL_SIZE 43

		DWORD IntelGMA_SoftList[ GMA_SL_SIZE ] = { 
			0x2782,0x2582,0x2792,0x2592,0x2772,0x2776,0x27A2,0x27A6,0x27AE,
			0x2982,0x2983,0x2992,0x2993,0x29A2,0x29A3,0x2972,0x2973,0x2A02,
			0x2A03,0x2A12,0x2A13,0x29C2,0x29C3,0x29B2,0x29B3,0x29D2,0x29D3,

			0x2A42,0x2A43,0x2E02,0x2E03,0x2E12,0x2E13,0x2E22,0x2E23,0x2E32,
			0x2E33,0x2E42,0x2E43,0x2E92,0x2E93,0x0042,0x0046
		};

		for ( int idx = 0 ; idx < GMA_SL_SIZE ; ++idx )
			if ( IntelGMA_SoftList[ idx ] == Caps.id_device ) {
				isIntelGMA = TRUE;
				break;
			}
	}

	if ( isIntelGMA )
		switch ( ps_r1_SoftwareSkinning ) {
			case 0 : 
				Msg( "* Enabling software skinning" );
				ps_r1_SoftwareSkinning = 1;
				break;
			case 1 : 
				Msg( "* Using software skinning" );
				break;
			case 2 : 
				Msg( "* WARNING: Using hardware skinning" );
				Msg( "*   setting 'r1_software_skinning' to '1' may improve performance" );
				break;
	} else
		if ( ps_r1_SoftwareSkinning == 1 ) {
				Msg( "* WARNING: Using software skinning" );
				Msg( "*   setting 'r1_software_skinning' to '0' should improve performance" );
		}

#endif // RENDER == R_R1

	if ( Caps.bForceGPU_SW ) 
		return D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DCAPS9	caps;
	pD3D->GetDeviceCaps(DevAdapter,DevT,&caps);

    if(caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		if (Caps.bForceGPU_NonPure)	return D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else {
			if (caps.DevCaps&D3DDEVCAPS_PUREDEVICE) return D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_PUREDEVICE;
			else return D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
		// return D3DCREATE_MIXED_VERTEXPROCESSING;
	} else return D3DCREATE_SOFTWARE_VERTEXPROCESSING;
}

u32 CHW::selectRefresh(u32 dwWidth, u32 dwHeight, D3DFORMAT fmt)
{
}

BOOL	CHW::support	(D3DFORMAT fmt, DWORD type, DWORD usage)
{
	HRESULT hr		= pD3D->CheckDeviceFormat(DevAdapter,DevT,Caps.fTarget,usage,(D3DRESOURCETYPE)type,fmt);
	if (FAILED(hr))	return FALSE;
	else			return TRUE;
}

void	CHW::updateWindowProps	(HWND m_hWnd)
{
}


struct _uniq_mode
{
	_uniq_mode(LPCSTR v):_val(v){}
	LPCSTR _val;
	bool operator() (LPCSTR _other) {return !stricmp(_val,_other);}
};

#ifndef REDITOR

/*
void free_render_mode_list()
{
	for( int i=0; vid_quality_token[i].name; i++ )
	{
		xr_free					(vid_quality_token[i].name);
	}
	xr_free						(vid_quality_token);
	vid_quality_token			= NULL;
}
*/
/*
void	fill_render_mode_list()
{
	if(vid_quality_token != NULL)		return;

	D3DCAPS9					caps;
	CHW							_HW;
	_HW.CreateD3D				();
	_HW.pD3D->GetDeviceCaps		(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
	_HW.DestroyD3D				();
	u16		ps_ver_major		= u16 ( u32(u32(caps.PixelShaderVersion)&u32(0xf << 8ul))>>8 );

	xr_vector<LPCSTR>			_tmp;
	u32 i						= 0;
	for(; i<5; ++i)
	{
		bool bBreakLoop = false;
		switch (i)
		{
		case 3:		//"renderer_r2.5"
			if (ps_ver_major < 3)
				bBreakLoop = true;
			break;
		case 4:		//"renderer_r_dx10"
			bBreakLoop = true;
			break;
		default:	;
		}

		if (bBreakLoop) break;

		_tmp.push_back				(NULL);
		LPCSTR val					= NULL;
		switch (i)
		{
			case 0: val ="renderer_r1";			break;
			case 1: val ="renderer_r2a";		break;
			case 2: val ="renderer_r2";			break;
			case 3: val ="renderer_r2.5";		break;
			case 4: val ="renderer_r_dx10";		break; //  -)
		}
		_tmp.back()					= xr_strdup(val);
	}
	u32 _cnt								= _tmp.size()+1;
	vid_quality_token						= xr_alloc<xr_token>(_cnt);

	vid_quality_token[_cnt-1].id			= -1;
	vid_quality_token[_cnt-1].name			= NULL;

#ifdef DEBUG
	Msg("Available render modes[%d]:",_tmp.size());
#endif // DEBUG
	for(u32 i=0; i<_tmp.size();++i)
	{
		vid_quality_token[i].id				= i;
		vid_quality_token[i].name			= _tmp[i];
#ifdef DEBUG
		Msg							("[%s]",_tmp[i]);
#endif // DEBUG
	}
}
*/
void free_vid_mode_list()
{
	for( int i=0; vid_mode_token[i].name; i++ )
	{
		xr_free					(vid_mode_token[i].name);
	}
	xr_free						(vid_mode_token);
	vid_mode_token				= NULL;
}

void fill_vid_mode_list(CHW* _hw)
{
	if(vid_mode_token != NULL)		return;
	xr_vector<LPCSTR>	_tmp;
	u32 cnt = _hw->pD3D->GetAdapterModeCount	(_hw->DevAdapter, _hw->Caps.fTarget);

    u32 i;
	for(i=0; i<cnt;++i)
	{
		D3DDISPLAYMODE	Mode;
		string32		str;

		_hw->pD3D->EnumAdapterModes(_hw->DevAdapter, _hw->Caps.fTarget, i, &Mode);
		if(Mode.Width < 800)		continue;

		xr_sprintf						(str,sizeof(str),"%dx%d", Mode.Width, Mode.Height);
	
		if(_tmp.end() != std::find_if(_tmp.begin(), _tmp.end(), _uniq_mode(str)))
			continue;

		_tmp.push_back				(NULL);
		_tmp.back()					= xr_strdup(str);
	}

	u32 _cnt						= _tmp.size()+1;

	vid_mode_token					= xr_alloc<xr_token>(_cnt);

	vid_mode_token[_cnt-1].id			= -1;
	vid_mode_token[_cnt-1].name		= NULL;

#ifdef DEBUG
	Msg("Available video modes[%d]:",_tmp.size());
#endif // DEBUG
	for(i=0; i<_tmp.size();++i)
	{
		vid_mode_token[i].id		= i;
		vid_mode_token[i].name		= _tmp[i];
#ifdef DEBUG
		Msg							("[%s]",_tmp[i]);
#endif // DEBUG
	}
}
#endif

