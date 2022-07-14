#include "stdafx.h"
#include "dxStatGraphRender.h"

void dxStatGraphRender::Copy(IStatGraphRender&_in)
{
	*this = *((dxStatGraphRender*)&_in);
}

void dxStatGraphRender::OnDeviceCreate()
{
	hGeomLine.create(FVF::F_TL0uv,RCache.Vertex.Buffer(),RCache.Index.Buffer());
	hGeomTri.create(FVF::F_TL0uv,RCache.Vertex.Buffer(),RCache.QuadIB);
}

void dxStatGraphRender::OnDeviceDestroy()
{
	hGeomLine.destroy();
	hGeomTri.destroy();
}

void dxStatGraphRender::OnRender(CStatGraph &owner)
{
}