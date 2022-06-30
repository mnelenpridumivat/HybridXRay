#include "stdafx.h"
#include "igame_level.h"
#include "igame_persistent.h"

#include "x_ray.h"
#include "std_classes.h"
#include "customHUD.h"
#include "render.h"
#include "gamefont.h"
#include "xrLevel.h"
#include "CameraManager.h"
#include "xr_object.h"

#include "securom_api.h"

#include "..\XrAPI\xrGameManager.h"

ENGINE_API	IGame_Level*	g_pGameLevel	= NULL;
extern	BOOL g_bLoaded;

IGame_Level::IGame_Level	()
{
	m_pCameras					= xr_new<CCameraManager>(true);
	g_pGameLevel				= this;
	pLevel						= NULL;
	bReady						= false;
	pCurrentEntity				= NULL;
	pCurrentViewEntity			= NULL;
	Device->DumpResourcesMemoryUsage();
}

//#include "resourcemanager.h"

IGame_Level::~IGame_Level	()
{
	if(strstr(Core.Params,"-nes_texture_storing") )
		//Device->Resources->StoreNecessaryTextures();
		Device->m_pRender->ResourcesStoreNecessaryTextures();
	xr_delete					( pLevel		);

	// Render-level unload
	Render->level_Unload		();
	xr_delete					(m_pCameras);
	// Unregister
	Device->seqRender.Remove		(this);
	Device->seqFrame.Remove		(this);
	CCameraManager::ResetPP		();
///////////////////////////////////////////
	Device->DumpResourcesMemoryUsage();

	u32		m_base=0,c_base=0,m_lmaps=0,c_lmaps=0;
	if (Device->m_pRender) 
		Device->m_pRender->ResourcesGetMemoryUsage(m_base,c_base,m_lmaps,c_lmaps);

	Msg		("* [ D3D ]: textures[%d K]", (m_base+m_lmaps)/1024);

}

void IGame_Level::net_Stop			()
{
	for (int i=0; i<6; i++)
		Objects.Update			(false);
	// Destroy all objects
	Objects.Unload				( );
	IR_Release					( );

	bReady						= false;	
}

//-------------------------------------------------------------------------------------------
//extern CStatTimer				tscreate;

BOOL IGame_Level::Load			(u32 dwNum) 
{
	SECUROM_MARKER_PERFORMANCE_ON(10)

	// Initialize level data
	pApp->Level_Set				( dwNum );
	string_path					temp;
	if (!FS.exist(temp, "$level$", "level.ltx"))
		Debug.fatal	(DEBUG_INFO,"Can't find level configuration file '%s'.",temp);
	pLevel						= xr_new<CInifile>	( temp );
	
	// Open
//	g_pGamePersistent->LoadTitle	("st_opening_stream");
	g_pGamePersistent->LoadTitle	();
	IReader* LL_Stream			= FS.r_open	("$level$","level");
	IReader	&fs					= *LL_Stream;

	// Header
	hdrLEVEL					H;
	fs.r_chunk_safe				(fsL_HEADER,&H,sizeof(H));
	R_ASSERT2					(XRCL_PRODUCTION_VERSION==H.XRLC_version,"Incompatible level version.");

	// CForms
//	g_pGamePersistent->LoadTitle	("st_loading_cform");
	g_pGamePersistent->LoadTitle	();
	//Sound->set_geometry_occ		( &Static );

	pApp->LoadSwitch			();


	// HUD + Environment
	if(!g_hud)
		g_hud					= (CCustomHUD*)NEW_INSTANCE	(CLSID_HUDMANAGER);

	// Render-level Load
	Render->level_Load			(LL_Stream);
	// tscreate.FrameEnd			();
	// Msg						("* S-CREATE: %f ms, %d times",tscreate.result,tscreate.count);

	// Objects
	g_pGamePersistent->Environment().mods_load	();
	R_ASSERT					(Load_GameSpecific_Before());
	Objects.Load();
	if (xrGameManager::GetGame() == EGame::SHOC)
	{
		R_ASSERT(Load_GameSpecific_After());
	}

	// Done
	FS.r_close					( LL_Stream );
	bReady						= true;
	if (!g_dedicated_server)	IR_Capture();
#ifndef DEDICATED_SERVER
	Device->seqRender.Add		(this);
#endif

	Device->seqFrame.Add			(this);

	SECUROM_MARKER_PERFORMANCE_OFF(10)

	return TRUE;	
}

int		psNET_DedicatedSleep	= 5;
void	IGame_Level::OnRender		( ) 
{
#ifndef DEDICATED_SERVER
//	if (_abs(Device->fTimeDelta)<EPS_S) return;

	// Level render, only when no client output required
	if (!g_dedicated_server)	{
		Render->Calculate			();
		Render->Render				();
	} else {
		Sleep						(psNET_DedicatedSleep);
	}

	// Font
//	pApp->pFontSystem->SetSizeI(0.023f);
//	pApp->pFontSystem->OnRender	();
#endif
}

void	IGame_Level::OnFrame		( ) 
{
	// Log				("- level:on-frame: ",u32(Device->dwFrame));
//	if (_abs(Device->fTimeDelta)<EPS_S) return;

	// Update all objects
	VERIFY						(bReady);
	Objects.Update				(false);
	g_hud->OnFrame				();
}
// ==================================================================================================

void CServerInfo::AddItem( LPCSTR name_, LPCSTR value_, u32 color_ )
{
	shared_str s_name( name_ );
	AddItem( s_name, value_, color_ );
}

void CServerInfo::AddItem( shared_str& name_, LPCSTR value_, u32 color_ )
{
	SItem_ServerInfo it;
	//	shared_str s_name = CStringTable().translate( name_ );

	//	xr_strcpy( it.name, s_name.c_str() );
	xr_strcpy( it.name, name_.c_str() );
	xr_strcat( it.name, " = " );
	xr_strcat( it.name, value_ );
	it.color = color_;

	if ( data.size() < max_item )
	{
		data.push_back( it );
	}
}

void IGame_Level::SetEntity( CObject* O  )
{
	if (pCurrentEntity)
		pCurrentEntity->On_LostEntity();
	
	if (O)
		O->On_SetEntity();

	pCurrentEntity=pCurrentViewEntity=O;
}

void IGame_Level::SetViewEntity( CObject* O  )
{
	if (pCurrentViewEntity)
		pCurrentViewEntity->On_LostEntity();

	if (O)
		O->On_SetEntity();

	pCurrentViewEntity=O;
}

void	IGame_Level::SoundEvent_Dispatch	( )
{
}

// Lain: added
void   IGame_Level::SoundEvent_OnDestDestroy (Feel::Sound* obj)
{
	struct rem_pred
	{
		rem_pred(Feel::Sound* obj) : m_obj(obj) {}

		bool operator () (const _esound_delegate& d)
		{
			return d.dest == m_obj;
		}

	private:
		Feel::Sound* m_obj;
	};

	snd_Events.erase( std::remove_if(snd_Events.begin(), snd_Events.end(), rem_pred(obj)),
	                  snd_Events.end() );
}


