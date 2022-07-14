#ifndef	dxStatGraphRender_included
#define	dxStatGraphRender_included
#pragma once

#include "StatGraphRender.h"

class dxStatGraphRender : public IStatGraphRender
{
public:
	virtual void Copy(IStatGraphRender &_in);

	virtual void OnDeviceCreate();
	virtual void OnDeviceDestroy();
	virtual void OnRender(CStatGraph &owner);

private:
	ref_geom 		hGeomTri;
	ref_geom 		hGeomLine;
};

#endif	//	dxStatGraphRender_included