#include "stdafx.h"
#include "dxThunderboltRender.h"

#include "dxThunderboltDescRender.h"
#include "dxLensFlareRender.h"

dxThunderboltRender::dxThunderboltRender()
{
	// geom
	hGeom_model.create	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, RCache.Vertex.Buffer(), RCache.Index.Buffer());
	hGeom_gradient.create(FVF::F_LIT,RCache.Vertex.Buffer(),RCache.QuadIB);
}

dxThunderboltRender::~dxThunderboltRender()
{
	hGeom_model.destroy();
	hGeom_gradient.destroy();
}

void dxThunderboltRender::Copy(IThunderboltRender &_in)
{
	*this = *(dxThunderboltRender*)&_in;
}

void dxThunderboltRender::Render(CEffect_Thunderbolt &owner)
{

}