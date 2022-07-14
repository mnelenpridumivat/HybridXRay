#include "stdafx.h"
#include "dxLensFlareRender.h"
#define MAX_Flares	24

#define FAR_DIST g_pGamePersistent->Environment().CurrentEnv->far_plane

void dxFlareRender::Copy(IFlareRender &_in)
{
	*this = *(dxFlareRender*)&_in;
}

void dxFlareRender::CreateShader(LPCSTR sh_name, LPCSTR tex_name)
{
	if (tex_name&&tex_name[0])	
		hShader.create(sh_name,tex_name);
}

void dxFlareRender::DestroyShader()
{
	hShader.destroy();
}

void dxLensFlareRender::Copy(ILensFlareRender &_in)
{
	*this = *(dxLensFlareRender*)&_in;
}

void dxLensFlareRender::Render(CLensFlare& owner, BOOL bSun, BOOL bFlares, BOOL bGradient)
{
}

void dxLensFlareRender::OnDeviceCreate()
{
	hGeom.create(FVF::F_LIT,RCache.Vertex.Buffer(),RCache.QuadIB);
}

void dxLensFlareRender::OnDeviceDestroy()
{
	hGeom.destroy();
}