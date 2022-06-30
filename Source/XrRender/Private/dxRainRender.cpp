#include "stdafx.h"
#include "dxRainRender.h"

#include "../../xrEngine/Rain.h"

//	Warning: duplicated in rain.cpp
static const int	max_desired_items	= 2500;
static const float	source_radius		= 12.5f;
static const float	source_offset		= 40.f;
static const float	max_distance		= source_offset*1.25f;
static const float	sink_offset			= -(max_distance-source_offset);
static const float	drop_length			= 5.f;
static const float	drop_width			= 0.30f;
static const float	drop_angle			= 3.0f;
static const float	drop_max_angle		= deg2rad(10.f);
static const float	drop_max_wind_vel	= 20.0f;
static const float	drop_speed_min		= 40.f;
static const float	drop_speed_max		= 80.f;

const int	max_particles		= 1000;
const int	particles_cache		= 400;
const float particles_time		= .3f;

dxRainRender::dxRainRender()
{
	IReader* F = FS.r_open("$game_meshes$","dm\\rain.dm"); 
	VERIFY3(F,"Can't open file.","dm\\rain.dm");

	DM_Drop	= ::RImplementation.model_CreateDM		(F);

	//
	SH_Rain.create("effects\\rain","fx\\fx_rain");
	hGeom_Rain.create(FVF::F_LIT, RCache.Vertex.Buffer(), RCache.QuadIB);
	hGeom_Drops.create(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, RCache.Vertex.Buffer(), RCache.Index.Buffer());
	
	FS.r_close(F);
}

dxRainRender::~dxRainRender()
{
	::RImplementation.model_Delete(DM_Drop);
}

void dxRainRender::Copy(IRainRender &_in)
{
	*this = *(dxRainRender*)&_in;
}

#include "../../xrEngine/iGame_persistent.h"

void dxRainRender::Render(CEffect_Rain &owner)
{
	
}

const Fsphere& dxRainRender::GetDropBounds() const
{
	return DM_Drop->bv_sphere;
}