#include "stdafx.h"
#include "xrserver.h"
#include "game_sv_single.h"
#include "game_sv_deathmatch.h"
#include "game_sv_teamdeathmatch.h"
#include "game_sv_artefacthunt.h"
#include "xrMessages.h"
#include "game_cl_artefacthunt.h"
#include "game_cl_single.h"
#include "MainMenu.h"
#include "../xrEngine/XRayEngineInterface.h"
#include "file_transfer.h"
#include "screenshot_server.h"
#include "player_name_modifyer.h"

#pragma warning(push)
#pragma warning(disable:4995)
#include <malloc.h>
#pragma warning(pop)

LPCSTR xrServer::get_map_download_url(LPCSTR level_name, LPCSTR level_version)
{
	R_ASSERT(level_name && level_version);
	LPCSTR ret_url = "";
	CInifile* level_ini = g_Engine->GetArchiveHeader(level_name, level_version);
	if (!level_ini)
	{
		if(!IsGameTypeSingle())
			Msg("! Warning: level [%s][%s] has not header ltx", level_name, level_version);

		return ret_url;
	}

	ret_url = level_ini->r_string_wb("header", "link").c_str();
	if (!ret_url)
		ret_url = "";
	
	return ret_url;
}

xrServer::EConnect xrServer::Connect(shared_str &session_name, GameDescriptionData & game_descr)
{
#ifdef DEBUG
	Msg						("* sv_Connect: %s",	*session_name);
#endif

	// Parse options and create game
	if (0==strchr(*session_name,'/'))
		return				ErrConnect;

	string1024				options;
	R_ASSERT2(xr_strlen(session_name) <= sizeof(options), "session_name too BIIIGGG!!!");
	strcpy_s					(options,strchr(*session_name,'/')+1);
	
	// Parse game type
	string1024				type;
	R_ASSERT2(xr_strlen(options) <= sizeof(type), "session_name too BIIIGGG!!!");
	strcpy_s					(type,options);
	if (strchr(type,'/'))	*strchr(type,'/') = 0;
	game					= NULL;

	CLASS_ID clsid			= game_GameState::getCLASS_ID(type,true);
	game					= smart_cast<game_sv_GameState*> (NEW_INSTANCE(clsid));
	
	// Options
	if (0==game)			return ErrConnect;
//	game->type				= type_id;
	if (game->Type() != eGameIDSingle)
	{
		m_file_transfers	= xr_new<file_transfer::server_site>();
		initialize_screenshot_proxies();
	}
#ifdef DEBUG
	Msg("* Created server_game %s",game->type_name());
#endif
	
	ZeroMemory(&game_descr, sizeof(game_descr));
	strcpy_s(game_descr.map_name, game->level_name(session_name.c_str()).c_str());
	strcpy_s(game_descr.map_version, game_sv_GameState::parse_level_version(session_name.c_str()).c_str());
	strcpy_s(game_descr.download_url, get_map_download_url(game_descr.map_name, game_descr.map_version));

	game->Create			(session_name);

#ifdef BATTLEYE
	if ( game->get_option_i( *session_name, "battleye", 1) != 0 ) // default => battleye enable (always)
	{
		// if level exist & if server in internet
		if ( g_pGameLevel && (game->get_option_i( *session_name, "public", 0) != 0)  )
		{
			if ( Level().battleye_system.server )
		{
				Msg( "Warning: BattlEye already loaded!" );
			}
			else
			{
				
		}
		}//g_pGameLevel
	}
/*	if ( g_pGameLevel && Level().battleye_system.server )
	{
		if ( game->get_option_i( *session_name, "battleye_update", 1) != 0 ) // default => battleye auto_update enable (always)
		{
			Level().battleye_system.auto_update = 1;
		}
		else
		{
			Level().battleye_system.auto_update = 0;
		}
	}*/
#endif // BATTLEYE
	
	return IPureServer::Connect(*session_name, game_descr);
}


IClient* xrServer::new_client( SClientConnectData* cl_data )
{
	IClient* CL		= client_Find_Get( cl_data->clientID );
	VERIFY( CL );
	
	// copy entity
	CL->ID			= cl_data->clientID;
	CL->process_id	= cl_data->process_id;
	
	string256 new_name;
	//strcpy_s( new_name, cl_data->name );
	modify_player_name(cl_data->name, new_name);
	CL->name._set( new_name );
	
	if ( game->NewPlayerName_Exists( CL, new_name ) )
	{
		if ( HasProtected() )
		{
			Msg( "! Server: Duplicated player`s name \"%s\" at protected server !", new_name );
		}
		else
		{
			game->NewPlayerName_Generate( CL, new_name );
			game->NewPlayerName_Replace( CL, new_name );
		}
	}
	CL->name._set( new_name );
	CL->pass._set( cl_data->pass );

	NET_Packet		P;
	P.B.count		= 0;
	P.r_pos			= 0;

	game->AddDelayedEvent( P, GAME_EVENT_CREATE_CLIENT, 0, CL->ID );
	/*if ( GetClientsCount() == 1 )
	{
		//multithreaded problem ...
		Update();			//incorect behavior (game_sv...:: m_event_queue can be corrupted !)
	}*/
	return CL;
}

void xrServer::AttachNewClient			(IClient* CL)
{
	MSYS_CONFIG	msgConfig;
	msgConfig.sign1 = 0x12071980;
	msgConfig.sign2 = 0x26111975;


	if(psNET_direct_connect) //single_game
	{
        SV_Client			= CL;
		CL->flags.bLocal	= 1;
		SendTo_LL( SV_Client->ID, &msgConfig, sizeof(msgConfig), net_flags(TRUE,TRUE,TRUE,TRUE) );
	}
	else
	{
		SendTo_LL				(CL->ID,&msgConfig,sizeof(msgConfig), net_flags(TRUE, TRUE, TRUE, TRUE));
		Server_Client_Check		(CL); 
	}

	// gen message
	if (!NeedToCheckClient_GameSpy_CDKey(CL))
	{
		//-------------------------------------------------------------
		Check_GameSpy_CDKey_Success(CL);
	}

	//xrClientData * CL_D=(xrClientData*)(CL); 
	//ip_address				ClAddress;
	//GetClientAddress		(CL->ID, ClAddress);
	CL->m_guid[0]=0;
}

void xrServer::RequestClientDigest(IClient* CL)
{
	if (IsGameTypeSingle())//(CL == GetServerClient())
	{
		Check_BuildVersion_Success(CL);	
		return;
	}
	xrClientData* tmp_client	= smart_cast<xrClientData*>(CL);
	VERIFY						(tmp_client);
	PerformSecretKeysSync		(tmp_client);

	NET_Packet P;
	P.w_begin					(M_SV_DIGEST);
	SendTo						(CL->ID, P);
}

#define NET_BANNED_STR	"Player banned by server!"
void xrServer::ProcessClientDigest(xrClientData* xrCL, NET_Packet* P)
{
	R_ASSERT(xrCL);
	IClient* tmp_client		= static_cast<IClient*>(xrCL);
	game_sv_mp* server_game = smart_cast<game_sv_mp*>(game);
	
	shared_str	admin_name;
	shared_str	secondary_cdkey;

	P->r_stringZ	(xrCL->m_cdkey_digest);
	P->r_stringZ	(secondary_cdkey);

	if (server_game->IsPlayerBanned(xrCL->m_cdkey_digest.c_str(), admin_name) ||
		server_game->IsPlayerBanned(secondary_cdkey.c_str(), admin_name))
	{
		R_ASSERT2(tmp_client != GetServerClient(), "can't disconnect server client");
		Msg("--- Client [%s][%s] tried to connect - rejecting connection (he is banned by %s) ...",
			tmp_client->m_cAddress.to_string().c_str(), tmp_client->name.c_str(),
			admin_name.size() ? admin_name.c_str() : "Server");
		LPSTR message_to_user;
		if (admin_name.size())
		{
			STRCONCAT(message_to_user, "You have been banned by ", admin_name.c_str());
		} else
		{
			message_to_user = "";
		}
		SendConnectResult(tmp_client, 0, 3, message_to_user);
		return;
	}
	PerformSecretKeysSync(xrCL);
	Check_BuildVersion_Success(tmp_client);	
}