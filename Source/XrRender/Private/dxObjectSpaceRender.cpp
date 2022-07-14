#include "stdafx.h"

#ifdef DEBUG

#include "dxObjectSpaceRender.h"

dxObjectSpaceRender::dxObjectSpaceRender()
{
	m_shDebug.create("debug\\wireframe","$null");
}

dxObjectSpaceRender::~dxObjectSpaceRender()
{
	m_shDebug.destroy();
}

void dxObjectSpaceRender::Copy (IObjectSpaceRender &_in)
{
	*this = *(dxObjectSpaceRender*)&_in;
}

void dxObjectSpaceRender::dbgAddSphere(const Fsphere &sphere, u32 colour)
{
	dbg_S.push_back	(mk_pair(sphere,colour));
}

void dxObjectSpaceRender::dbgRender()
{
}

void dxObjectSpaceRender::SetShader()
{
	RCache.set_Shader(m_shDebug);
}

#endif // DEBUG