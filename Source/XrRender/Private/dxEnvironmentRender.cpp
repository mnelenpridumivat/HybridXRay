#include "stdafx.h"
#include "dxEnvironmentRender.h"

#include "dxRenderDeviceRender.h"

#include "../Private/ResourceManager.h"


//////////////////////////////////////////////////////////////////////////
// half box def
static	Fvector3	hbox_verts[24]	=
{
	{-1.f,	-1.f,	-1.f}, {-1.f,	-1.01f,	-1.f},	// down
	{ 1.f,	-1.f,	-1.f}, { 1.f,	-1.01f,	-1.f},	// down
	{-1.f,	-1.f,	 1.f}, {-1.f,	-1.01f,	 1.f},	// down
	{ 1.f,	-1.f,	 1.f}, { 1.f,	-1.01f,	 1.f},	// down
	{-1.f,	 2.f,	-1.f}, {-1.f,	 1.f,	-1.f},
	{ 1.f,	 2.f,	-1.f}, { 1.f,	 1.f,	-1.f},
	{-1.f,	 2.f,	 1.f}, {-1.f,	 1.f,	 1.f},
	{ 1.f,	 2.f,	 1.f}, { 1.f,	 1.f,	 1.f},
	{-1.f,	 0.f,	-1.f}, {-1.f,	-1.f,	-1.f},	// half
	{ 1.f,	 0.f,	-1.f}, { 1.f,	-1.f,	-1.f},	// half
	{ 1.f,	 0.f,	 1.f}, { 1.f,	-1.f,	 1.f},	// half
	{-1.f,	 0.f,	 1.f}, {-1.f,	-1.f,	 1.f}	// half
};
static	u16			hbox_faces[20*3]	=
{
	0,	 2,	 3,
	3,	 1,	 0,
	4,	 5,	 7,
	7,	 6,	 4,
	0,	 1,	 9,
	9,	 8,	 0,
	8,	 9,	 5,
	5,	 4,	 8,
	1,	 3,	10,
	10,	 9,	 1,
	9,	10,	 7,
	7,	 5,	 9,
	3,	 2,	11,
	11,	10,	 3,
	10,	11,	 6,
	6,	 7,	10,
	2,	 0,	 8,
	8,	11,	 2,
	11,	 8,	 4,
	4,	 6,	11
};

#pragma pack(push,1)
struct v_skybox				{
	Fvector3	p;
	u32			color;
	Fvector3	uv	[2];

	void		set			(Fvector3& _p, u32 _c, Fvector3& _tc)
	{
		p					= _p;
		color				= _c;
		uv[0]				= _tc;
		uv[1]				= _tc;
	}
};
const	u32 v_skybox_fvf	= D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE3(0) | D3DFVF_TEXCOORDSIZE3(1);
struct v_clouds				{
	Fvector3	p;
	u32			color;
	u32			intensity;
	void		set			(Fvector3& _p, u32 _c, u32 _i)
	{
		p					= _p;
		color				= _c;
		intensity			= _i;
	}
};
const	u32 v_clouds_fvf	= D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR;
#pragma pack(pop)

void dxEnvDescriptorRender::Copy(IEnvDescriptorRender &_in)
{
	*this = *(dxEnvDescriptorRender*)&_in;
}

void dxEnvDescriptorMixerRender::Copy(IEnvDescriptorMixerRender &_in)
{
	*this = *(dxEnvDescriptorMixerRender*)&_in;
}

void dxEnvironmentRender::Copy(IEnvironmentRender &_in)
{
	*this = *(dxEnvironmentRender*)&_in;
}

particles_systems::library_interface const& dxEnvironmentRender::particles_systems_library()
{
	particles_systems::library_interface* inter = nullptr;
	return *inter;
}

void dxEnvDescriptorMixerRender::Destroy()
{
	sky_r_textures.clear		();
	sky_r_textures_env.clear	();
	clouds_r_textures.clear		();
}

void dxEnvDescriptorMixerRender::Clear()
{
	std::pair<u32,ref_texture>	zero = mk_pair(u32(0),ref_texture(0));
	sky_r_textures.clear		();
	sky_r_textures.push_back	(zero);
	sky_r_textures.push_back	(zero);
	sky_r_textures.push_back	(zero);

	sky_r_textures_env.clear	();
	sky_r_textures_env.push_back(zero);
	sky_r_textures_env.push_back(zero);
	sky_r_textures_env.push_back(zero);

	clouds_r_textures.clear		();
	clouds_r_textures.push_back	(zero);
	clouds_r_textures.push_back	(zero);
	clouds_r_textures.push_back	(zero);
}

void dxEnvDescriptorMixerRender::lerp(IEnvDescriptorRender *inA, IEnvDescriptorRender *inB)
{
	dxEnvDescriptorRender *pA = (dxEnvDescriptorRender *)inA;
	dxEnvDescriptorRender *pB = (dxEnvDescriptorRender *)inB;

	sky_r_textures.clear		();
	sky_r_textures.push_back	(mk_pair(0,pA->sky_texture));
	sky_r_textures.push_back	(mk_pair(1,pB->sky_texture));

	sky_r_textures_env.clear	();

	sky_r_textures_env.push_back(mk_pair(0,pA->sky_texture_env));
	sky_r_textures_env.push_back(mk_pair(1,pB->sky_texture_env));

	clouds_r_textures.clear		();
	clouds_r_textures.push_back	(mk_pair(0,pA->clouds_texture));
	clouds_r_textures.push_back	(mk_pair(1,pB->clouds_texture));
}

void dxEnvDescriptorRender::OnDeviceCreate(IEnvDescriptor &owner)
{
}

void dxEnvDescriptorRender::OnDeviceDestroy()
{
	sky_texture.destroy		();
	sky_texture_env.destroy	();
	clouds_texture.destroy	();
}

dxEnvironmentRender::dxEnvironmentRender()
{
	tsky0	= DEV->_CreateTexture("$user$sky0");
	tsky1	= DEV->_CreateTexture("$user$sky1");
}

void dxEnvironmentRender::OnFrame(IEnvironment &env)
{
}

void dxEnvironmentRender::OnLoad()
{
	tonemap	= DEV->_CreateTexture("$user$tonemap");	//. hack
}

void dxEnvironmentRender::OnUnload()
{
	tonemap	= 0;
}

void dxEnvironmentRender::RenderSky(IEnvironment &env)
{
}

void dxEnvironmentRender::RenderClouds(IEnvironment &env)
{
}

void dxEnvironmentRender::OnDeviceCreate()
{
	sh_2sky.create			(&m_b_skybox,"skybox_2t");
	sh_2geom.create			(v_skybox_fvf,RCache.Vertex.Buffer(), RCache.Index.Buffer());
	clouds_sh.create		("clouds","null");
	clouds_geom.create		(v_clouds_fvf,RCache.Vertex.Buffer(), RCache.Index.Buffer());
}

void dxEnvironmentRender::OnDeviceDestroy()
{
	tsky0->surface_set						(NULL);
	tsky1->surface_set						(NULL);

	sh_2sky.destroy							();
	sh_2geom.destroy						();
	clouds_sh.destroy						();
	clouds_geom.destroy						();
}