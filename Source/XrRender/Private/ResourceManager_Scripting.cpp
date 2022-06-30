#include "stdafx.h"
#pragma hdrstop

#include <lua/library_linkage.h>

#include	"../../xrEngine/Render.h"
#include	"ResourceManager.h"
#include	"tss.h"
#include	"blenders\blender.h"
#include	"blenders\blender_recorder.h"
#include	"../../xrEngine/ai_script_space.h"
#include	"../../xrEngine/ai_script_lua_extension.h"
#include	"luabind/return_reference_to_policy.hpp"

#include	"dxRenderDeviceRender.h"
#include "../../XrCore/doug_lea_allocator.h"

#if REDITOR
#include "Editor/render.h"
#define RESOURCE EDevice->Resources
#else
#define RESOURCE dxRenderDeviceRender::Instance().Resources
#endif

#ifdef	DEBUG
#define MDB	Memory.dbg_check()
#else
#define MDB
#endif

// wrapper
class	adopt_sampler
{
	CBlender_Compile*		C;
	u32						stage;
public:
	adopt_sampler			(CBlender_Compile*	_C, u32 _stage)		: C(_C), stage(_stage)		{ if (u32(-1)==stage) C=0;		}
	adopt_sampler			(const adopt_sampler&	_C)				: C(_C.C), stage(_C.stage)	{ if (u32(-1)==stage) C=0;		}

	adopt_sampler&			_texture		(LPCSTR texture)		{ if (C) C->i_Texture	(stage,texture);											return *this;	}
	adopt_sampler&			_projective		(bool _b)				{ if (C) C->i_Projective(stage,_b);													return *this;	}
	adopt_sampler&			_clamp			()						{ if (C) C->i_Address	(stage,D3DTADDRESS_CLAMP);									return *this;	}
	adopt_sampler&			_wrap			()						{ if (C) C->i_Address	(stage,D3DTADDRESS_WRAP);									return *this;	}
	adopt_sampler&			_mirror			()						{ if (C) C->i_Address	(stage,D3DTADDRESS_MIRROR);									return *this;	}
	adopt_sampler&			_f_anisotropic	()						{ if (C) C->i_Filter	(stage,D3DTEXF_ANISOTROPIC,D3DTEXF_LINEAR,D3DTEXF_ANISOTROPIC);	return *this;	}
	adopt_sampler&			_f_trilinear	()						{ if (C) C->i_Filter	(stage,D3DTEXF_LINEAR,D3DTEXF_LINEAR,D3DTEXF_LINEAR);		return *this;	}
	adopt_sampler&			_f_bilinear		()						{ if (C) C->i_Filter	(stage,D3DTEXF_LINEAR,D3DTEXF_POINT, D3DTEXF_LINEAR);		return *this;	}
	adopt_sampler&			_f_linear		()						{ if (C) C->i_Filter	(stage,D3DTEXF_LINEAR,D3DTEXF_NONE,  D3DTEXF_LINEAR);		return *this;	}
	adopt_sampler&			_f_none			()						{ if (C) C->i_Filter	(stage,D3DTEXF_POINT, D3DTEXF_NONE,  D3DTEXF_POINT);		return *this;	}
	adopt_sampler&			_fmin_none		()						{ if (C) C->i_Filter_Min(stage,D3DTEXF_NONE);										return *this;	}
	adopt_sampler&			_fmin_point		()						{ if (C) C->i_Filter_Min(stage,D3DTEXF_POINT);										return *this;	}
	adopt_sampler&			_fmin_linear	()						{ if (C) C->i_Filter_Min(stage,D3DTEXF_LINEAR);										return *this;	}
	adopt_sampler&			_fmin_aniso		()						{ if (C) C->i_Filter_Min(stage,D3DTEXF_ANISOTROPIC);								return *this;	}
	adopt_sampler&			_fmip_none		()						{ if (C) C->i_Filter_Mip(stage,D3DTEXF_NONE);										return *this;	}
	adopt_sampler&			_fmip_point		()						{ if (C) C->i_Filter_Mip(stage,D3DTEXF_POINT);										return *this;	}
	adopt_sampler&			_fmip_linear	()						{ if (C) C->i_Filter_Mip(stage,D3DTEXF_LINEAR);										return *this;	}
	adopt_sampler&			_fmag_none		()						{ if (C) C->i_Filter_Mag(stage,D3DTEXF_NONE);										return *this;	}
	adopt_sampler&			_fmag_point		()						{ if (C) C->i_Filter_Mag(stage,D3DTEXF_POINT);										return *this;	}
	adopt_sampler&			_fmag_linear	()						{ if (C) C->i_Filter_Mag(stage,D3DTEXF_LINEAR);										return *this;	}
};																																							
																																							
// wrapper																																					
class	adopt_compiler																																		
{
	CBlender_Compile*		C;
public:
	adopt_compiler			(CBlender_Compile*	_C)	: C(_C)							{ }
	adopt_compiler			(const adopt_compiler&	_C)	: C(_C.C)					{ }

	adopt_compiler&			_options		(int	P,		bool	S)				{	C->SetParams		(P,S);					return	*this;		}
	adopt_compiler&			_o_emissive		(bool	E)								{	C->SH->flags.bEmissive=E;					return	*this;		}
	adopt_compiler&			_o_distort		(bool	E)								{	C->SH->flags.bDistort=E;					return	*this;		}
	adopt_compiler&			_o_wmark		(bool	E)								{	C->SH->flags.bWmark=E;						return	*this;		}
	adopt_compiler&			_pass			(LPCSTR	vs,		LPCSTR ps)				{	C->r_Pass			(vs,ps,true);			return	*this;		}
	adopt_compiler&			_fog			(bool	_fog)							{	C->PassSET_LightFog	(FALSE,_fog);			return	*this;		}
	adopt_compiler&			_ZB				(bool	_test,	bool _write)			{	C->PassSET_ZB		(_test,_write);			return	*this;		}
	adopt_compiler&			_blend			(bool	_blend, u32 abSRC, u32 abDST)	{	C->PassSET_ablend_mode(_blend,abSRC,abDST);	return 	*this;		}
	adopt_compiler&			_aref			(bool	_aref,  u32 aref)				{	C->PassSET_ablend_aref(_aref,aref);			return 	*this;		}
	adopt_compiler&			_color_write_enable (bool cR, bool cG, bool cB, bool cA)		{	C->r_ColorWriteEnable(cR, cG, cB, cA);		return	*this;		}
	adopt_sampler			_sampler		(LPCSTR _name)							{	u32 s = C->r_Sampler(_name,0);				return	adopt_sampler(C,s);	}
};

class	adopt_blend
{
public:
};

void LuaLog(LPCSTR caMessage)
{
	MDB;	
	Lua::LuaOut	(Lua::eLuaMessageTypeMessage,"%s",caMessage);
}
void LuaError(lua_State* L)
{
}

#ifndef PURE_ALLOC
//#	ifndef USE_MEMORY_MONITOR
#		define USE_DL_ALLOCATOR
//#	endif // USE_MEMORY_MONITOR
#endif // PURE_ALLOC

#ifndef USE_DL_ALLOCATOR
static void *lua_alloc_dl	(void *ud, void *ptr, size_t osize, size_t nsize) {
	(void)ud;
	(void)osize;
	if (nsize == 0) {
		xr_free	(ptr);
		return	NULL;
	}
	else
#ifdef DEBUG_MEMORY_NAME
		return Memory.mem_realloc		(ptr, nsize, "LUA");
#else // DEBUG_MEMORY_MANAGER
		return Memory.mem_realloc		(ptr, nsize);
#endif // DEBUG_MEMORY_MANAGER
}
#else // USE_DL_ALLOCATOR

#include "../../xrCore/memory_allocator_options.h"

#ifdef USE_ARENA_ALLOCATOR
	static const u32	s_arena_size = 8*1024*1024;
	static char			s_fake_array[s_arena_size];
	doug_lea_allocator	g_render_lua_allocator( s_fake_array, s_arena_size, "render:lua" );
#else // #ifdef USE_ARENA_ALLOCATOR
	doug_lea_allocator	g_render_lua_allocator( 0, 0, "render:lua" );
#endif // #ifdef USE_ARENA_ALLOCATOR

static void *lua_alloc		(void *ud, void *ptr, size_t osize, size_t nsize) {
#ifndef USE_MEMORY_MONITOR
	(void)ud;
	(void)osize;
	if ( !nsize )	{
		g_render_lua_allocator.free_impl	(ptr);
		return					0;
	}

	if ( !ptr )
		return					g_render_lua_allocator.malloc_impl((u32)nsize);

	return g_render_lua_allocator.realloc_impl(ptr, (u32)nsize);
#else // #ifndef USE_MEMORY_MONITOR
	if ( !nsize )	{
		memory_monitor::monitor_free(ptr);
		g_render_lua_allocator.free_impl		(ptr);
		return						NULL;
	}

	if ( !ptr ) {
		void* const result			= 
			g_render_lua_allocator.malloc_impl((u32)nsize);
		memory_monitor::monitor_alloc (result,nsize,"render:LUA");
		return						result;
	}

	memory_monitor::monitor_free	(ptr);
	void* const result				= g_render_lua_allocator.realloc_impl(ptr, (u32)nsize);
	memory_monitor::monitor_alloc	(result,nsize,"render:LUA");
	return							result;
#endif // #ifndef USE_MEMORY_MONITOR
}
#endif // USE_DL_ALLOCATOR

// export
void	CResourceManager::LS_Load			()
{

}

void	CResourceManager::LS_Unload			()
{
	LSVM		= NULL;
}

BOOL	CResourceManager::_lua_HasShader	(LPCSTR s_shader)
{
	return FALSE;
}

Shader*	CResourceManager::_lua_Create		(LPCSTR d_shader, LPCSTR s_textures)
{
	Shader S;
	return &S;
}

ShaderElement*		CBlender_Compile::_lua_Compile	(LPCSTR namesp, LPCSTR name)
{
	ShaderElement		E;
	return &E;
}
