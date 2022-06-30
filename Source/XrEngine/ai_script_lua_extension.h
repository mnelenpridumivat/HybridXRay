////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_script_lua_extension.h
//	Created 	: 19.09.2003
//  Modified 	: 22.09.2003
//	Author		: Dmitriy Iassenev
//	Description : XRay Script extensions
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ai_script_space.h"
//struct CLuaVirtualMachine;

namespace Script {
#ifndef ENGINE_BUILD
	void				vfExportGlobals				(CLuaVirtualMachine *tpLuaVM);
	void				vfExportFvector				(CLuaVirtualMachine *tpLuaVM);
	void				vfExportFmatrix				(CLuaVirtualMachine *tpLuaVM);
	void				vfExportGame				(CLuaVirtualMachine *tpLuaVM);
	void				vfExportLevel				(CLuaVirtualMachine *tpLuaVM);
	void				vfExportDevice				(CLuaVirtualMachine *tpLuaVM);
	void				vfExportParticles			(CLuaVirtualMachine *tpLuaVM);
	void				vfExportSound				(CLuaVirtualMachine *tpLuaVM);
	void				vfExportHit					(CLuaVirtualMachine *tpLuaVM);
	void				vfExportActions				(CLuaVirtualMachine *tpLuaVM);
	void				vfExportObject				(CLuaVirtualMachine *tpLuaVM);
	void				vfExportEffector			(CLuaVirtualMachine *tpLuaVM);
	void				vfExportArtifactMerger		(CLuaVirtualMachine *tpLuaVM);
	void				vfLoadStandardScripts		(CLuaVirtualMachine *tpLuaVM);
	void				vfExportMemoryObjects		(CLuaVirtualMachine *tpLuaVM);
	void				vfExportToLua				(CLuaVirtualMachine *tpLuaVM);
	void				vfExportActionManagement	(CLuaVirtualMachine *tpLuaVM);
	void				vfExportMotivationManagement(CLuaVirtualMachine *tpLuaVM);
	bool				bfLoadFile					(CLuaVirtualMachine *tpLuaVM, LPCSTR	caScriptName,	bool	bCall = true);
	void				LuaHookCall					(CLuaVirtualMachine *tpLuaVM, lua_Debug *tpLuaDebug);
	int					LuaPanic					(CLuaVirtualMachine *tpLuaVM);
#endif
	LPCSTR				cafEventToString			(int				iEventCode);
};
