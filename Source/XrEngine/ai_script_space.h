////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_script_space.h
//	Created 	: 22.09.2003
//  Modified 	: 22.09.2003
//	Author		: Dmitriy Iassenev
//	Description : XRay Script space
////////////////////////////////////////////////////////////////////////////

#pragma once

// Lua
#pragma warning(disable:4244)
#pragma warning(disable:4995)
#pragma warning(disable:4530)
#pragma warning(disable:4267)

//#define LUABIND_NO_EXCEPTIONS

// Lua-bind

#pragma warning(default:4244)
#pragma warning(default:4995)
#pragma warning(default:4267)

struct SMemberCallback {
	shared_str				m_method_name;
};

#include "ai_script_lua_space.h"
