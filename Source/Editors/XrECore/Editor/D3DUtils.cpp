// file: D3DUtils.cpp

#include "stdafx.h"
#pragma hdrstop

#include "d3dutils.h"
#include "du_box.h"
#include "du_sphere.h"
#include "du_sphere_part.h"
#include "du_cone.h"
#include "du_cylinder.h"
#include "Library.h"
#include "EditObject.h"
#include "ui_main.h"

#pragma warning(push)
#pragma warning(disable:4995)
#include "directx\d3dx9.h"
#pragma warning(pop)

#define LINE_DIVISION  32  // не меньше 6!!!!!
// for drawing sphere
static Fvector circledef1[LINE_DIVISION];
static Fvector circledef2[LINE_DIVISION];
static Fvector circledef3[LINE_DIVISION];

const u32 boxcolor = D3DCOLOR_RGBA(255,255,255,0);
static const int boxvertcount = 48;
static Fvector boxvert[boxvertcount];

#if 1
#	define DU_DRAW_RS	EDevice->SetRS
#	define DU_DRAW_SH_C(a,c){EDevice->SetShader(a);EDevice->SetRS(D3DRS_TEXTUREFACTOR,c);}
#	define DU_DRAW_SH(a){EDevice->SetShader(a);EDevice->SetRS(D3DRS_TEXTUREFACTOR,0xFFFFFFFF);}
#else
#	define DU_DRAW_RS	RCache.dbg_SetRS
#	define DU_DRAW_SH_C(sh,c){RCache.set_Shader(sh);	RCache.set_c	("tfactor",float(color_get_R(c))/255.f,float(color_get_G(c))/255.f,float(color_get_B(c))/255.f,float(color_get_A(c))/255.f);}
#	define DU_DRAW_SH(sh){RCache.set_Shader(sh);		RCache.set_c	("tfactor",1,1,1,1);}
#endif

#if 1
#	define FILL_MODE EDevice->dwFillMode
#	define SHADE_MODE EDevice->dwShadeMode
#	define SCREEN_QUALITY EDevice->m_ScreenQuality
#else
#	define FILL_MODE D3DFILL_SOLID
#	define SHADE_MODE D3DSHADE_GOURAUD
#	define SCREEN_QUALITY 1.f
#endif


// identity box
const u32 identboxcolor = D3DCOLOR_RGBA(255,255,255,0);
static const int identboxwirecount = 24;
static Fvector identboxwire[identboxwirecount] = {
	{-0.5f, -0.5f, -0.5f},	{-0.5f, +0.5f, -0.5f},    	{-0.5f, +0.5f, -0.5f},	{+0.5f, +0.5f, -0.5f},
    {+0.5f, +0.5f, -0.5f},	{+0.5f, -0.5f, -0.5f},		{+0.5f, -0.5f, -0.5f},	{-0.5f, -0.5f, -0.5f},
    {-0.5f, +0.5f, +0.5f},	{+0.5f, +0.5f, +0.5f},		{+0.5f, +0.5f, +0.5f},	{+0.5f, -0.5f, +0.5f},
    {+0.5f, -0.5f, +0.5f},	{-0.5f, -0.5f, +0.5f},    	{-0.5f, -0.5f, +0.5f},	{-0.5f, +0.5f, +0.5f},
    {-0.5f, +0.5f, -0.5f},	{-0.5f, +0.5f, +0.5f},		{+0.5f, +0.5f, -0.5f},	{+0.5f, +0.5f, +0.5f},
    {+0.5f, -0.5f, -0.5f},	{+0.5f, -0.5f, +0.5f},		{-0.5f, -0.5f, -0.5f},	{-0.5f, -0.5f, +0.5f}
};

/*
static const int identboxindexcount = 36;
static const WORD identboxindices[identboxindexcount] = {
	0, 1, 2,   2, 3, 0,
	3, 2, 6,   6, 7, 3,
	6, 4, 5,   6, 5, 7,
    4, 1, 5,   1, 0, 5,
    3, 5, 0,   3, 7, 5,
    1, 4, 6,   1, 6, 2};
static const int identboxindexwirecount = 24;
static const WORD identboxindiceswire[identboxindexwirecount] = {
	0, 1, 	1, 2,
	2, 3, 	3, 0,
	4, 6, 	6, 7,
    7, 5, 	5, 4,
    1, 4, 	2, 6,
    3, 7, 	0, 5};
*/

#define SIGN(x) ((x<0)?-1:1)

DEFINE_VECTOR(FVF::L,FLvertexVec,FLvertexIt)

static FLvertexVec 	m_GridPoints;

u32 m_ColorAxis	= 0xff000000;
u32 m_ColorGrid	= 0xff909090;
u32 m_ColorGridTh = 0xffb4b4b4;
u32 m_SelectionRect=D3DCOLOR_RGBA(127,255,127,64);

u32 m_ColorSafeRect = 0xffB040B0;

void 			 SPrimitiveBuffer::RenderDIP() { DU_DRAW_DIP(p_type, pGeom, 0, 0, v_cnt, 0, p_cnt); }
void SPrimitiveBuffer::CreateFromData(D3DPRIMITIVETYPE _pt, u32 _p_cnt, u32 FVF, LPVOID vertices, u32 _v_cnt, u16* indices, u32 _i_cnt)
{
}
void SPrimitiveBuffer::Destroy()
{                       
	if (pGeom){
		_RELEASE		(pGeom->vb);
		_RELEASE		(pGeom->ib);
		pGeom.destroy	();
	}
}