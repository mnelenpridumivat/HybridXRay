////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_script_lua_extension.cpp
//	Created 	: 19.09.2003
//  Modified 	: 19.09.2003
//	Author		: Dmitriy Iassenev
//	Description : XRay Script extensions
////////////////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include "stdafx.h"
#include "ai_script_lua_extension.h"
#include "ai_script_space.h"

#ifdef XRRENDER_R4_EXPORTS
#define ENGINE_BUILD
#endif	//	XRRENDER_R4_EXPORTS

#ifdef XRRENDER_R3_EXPORTS
#define ENGINE_BUILD
#endif	//	XRRENDER_R3_EXPORTS

#ifdef XRRENDER_R2_EXPORTS
#define ENGINE_BUILD
#endif	//	XRRENDER_R2_EXPORTS

#ifdef XRRENDER_R1_EXPORTS
#define ENGINE_BUILD
#endif	//	XRRENDER_R1_EXPORTS

#ifdef XRECORE_EXPORTS
#define ENGINE_BUILD
#endif	//	XRRENDER_R1_EXPORTS

#ifndef ENGINE_BUILD
	#include "../../XrServerEntities/ai_space.h"
#endif

using namespace Script;

int __cdecl Lua::LuaOut(Lua::ELuaMessageType tLuaMessageType, LPCSTR caFormat, ...)
{
#ifndef ENGINE_BUILD
	if (!psAI_Flags.test(aiLua))
		return(0);
#endif

	LPCSTR		S = "", SS = "";
	LPSTR		S1;
	string4096	S2;
	switch (tLuaMessageType) {
		case Lua::eLuaMessageTypeInfo : {
			S	= "* [LUA] ";
			SS	= "[INFO]        ";
			break;
		}
		case Lua::eLuaMessageTypeError : {
			S	= "! [LUA] ";
			SS	= "[ERROR]       ";
			break;
		}
		case Lua::eLuaMessageTypeMessage : {
			S	= "[LUA] ";
			SS	= "[MESSAGE]     ";
			break;
		}
		case Lua::eLuaMessageTypeHookCall : {
			S	= "[LUA][HOOK_CALL] ";
			SS	= "[CALL]        ";
			break;
		}
		case Lua::eLuaMessageTypeHookReturn : {
			S	= "[LUA][HOOK_RETURN] ";
			SS	= "[RETURN]      ";
			break;
		}
		case Lua::eLuaMessageTypeHookLine : {
			S	= "[LUA][HOOK_LINE] ";
			SS	= "[LINE]        ";
			break;
		}
		case Lua::eLuaMessageTypeHookCount : {
			S	= "[LUA][HOOK_COUNT] ";
			SS	= "[COUNT]       ";
			break;
		}
		case Lua::eLuaMessageTypeHookTailReturn : {
			S	= "[LUA][HOOK_TAIL_RETURN] ";
			SS	= "[TAIL_RETURN] ";
			break;
		}
		default : NODEFAULT;
	}
	
	va_list	l_tMarker;
	
	va_start(l_tMarker,caFormat);

	xr_strcpy	(S2,S);
	S1		= S2 + xr_strlen(S);
	int		l_iResult = vsprintf(S1,caFormat,l_tMarker);
	Msg		("%s",S2);
	
	xr_strcpy	(S2,SS);
	S1		= S2 + xr_strlen(SS);
	vsprintf(S1,caFormat,l_tMarker);

#ifdef ENGINE_BUILD
	// Msg("[LUA Output] : %s",S2);
#else
	ai().lua_output().w_string(S2);
#endif

	va_end	(l_tMarker);

	return	(l_iResult);
}

#ifndef ENGINE_BUILD
void Script::vfLoadStandardScripts(CLuaVirtualMachine *tpLuaVM)
{
	string256		S,S1;
	FS.update_path	(S,"$game_data$","script.ltx");
	CInifile		*l_tpIniFile = xr_new<CInifile>(S);
	R_ASSERT		(l_tpIniFile);
	LPCSTR			caScriptString = l_tpIniFile->r_string("common","script");

	u32				caNamespaceName = _GetItemCount(caScriptString);
	string256		I;
	for (u32 i=0; i<caNamespaceName; ++i) {
		FS.update_path(S,"$game_scripts$",strconcat(S1,_GetItem(caScriptString,i,I),".script"));
		bfLoadFile	(tpLuaVM,S,true);
		if (bfIsObjectPresent(tpLuaVM,"_G",xr_strcat(I,"_initialize"),LUA_TFUNCTION))
			lua_dostring(tpLuaVM,xr_strcat(I,"()"));
	}
	xr_delete		(l_tpIniFile);
}

void LuaError(lua_State* L)
{
	Debug.fatal(DEBUG_INFO,"LUA error: %s",lua_tostring(L,-1));
}

void Script::vfExportToLua(CLuaVirtualMachine *tpLuaVM)
{
	luabind::open					(tpLuaVM);
	luabind::set_error_callback		(LuaError);

	lua_atpanic		(tpLuaVM,Script::LuaPanic);

	vfExportGlobals				(tpLuaVM);
	vfExportFvector				(tpLuaVM);
	vfExportFmatrix				(tpLuaVM);
	vfExportGame				(tpLuaVM);
	vfExportLevel				(tpLuaVM);
	vfExportDevice				(tpLuaVM);
	vfExportParticles			(tpLuaVM);
	vfExportSound				(tpLuaVM);
	vfExportHit					(tpLuaVM);
	vfExportActions				(tpLuaVM);
	vfExportObject				(tpLuaVM);
	vfExportEffector			(tpLuaVM);
	vfExportArtifactMerger		(tpLuaVM);
	vfExportMemoryObjects		(tpLuaVM);
	vfExportActionManagement	(tpLuaVM);
	vfExportMotivationManagement(tpLuaVM);

#ifdef DEBUG
	lua_sethook					(tpLuaVM, LuaHookCall,	LUA_HOOKCALL | LUA_HOOKRET | LUA_HOOKLINE | LUA_HOOKTAILRET,	0);
#endif

	vfLoadStandardScripts		(tpLuaVM);
}

bool Script::bfLoadFile(CLuaVirtualMachine *tpLuaVM, LPCSTR caScriptName, bool bCall)
{
	string256		l_caNamespaceName;
	_splitpath		(caScriptName,0,0,l_caNamespaceName,0);
	if (!xr_strlen(l_caNamespaceName))
		return		(bfLoadFileIntoNamespace(tpLuaVM,caScriptName,"_G",bCall));
	else
		return		(bfLoadFileIntoNamespace(tpLuaVM,caScriptName,l_caNamespaceName,bCall));
}
#endif