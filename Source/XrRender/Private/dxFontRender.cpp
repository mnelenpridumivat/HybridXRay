#include "stdafx.h"
#include "dxFontRender.h"

dxFontRender::dxFontRender()
{

}

dxFontRender::~dxFontRender()
{
	pShader.destroy();
	pGeom.destroy();
}

void dxFontRender::Initialize(LPCSTR cShader, LPCSTR cTexture)
{
	pShader.create(cShader, cTexture);
	pGeom.create(FVF::F_TL, RCache.Vertex.Buffer(), RCache.QuadIB);
}
extern ENGINE_API BOOL g_bRendering;
extern ENGINE_API Fvector2		g_current_font_scale;
void dxFontRender::OnRender(CGameFont &owner)
{

}