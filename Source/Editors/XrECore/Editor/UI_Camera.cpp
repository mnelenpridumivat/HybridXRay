#include "stdafx.h"
#pragma hdrstop

#include "UI_Camera.h"
#include "ui_main.h"
#include "ui_toolscustom.h"

CUI_Camera::CUI_Camera()
{
	m_Style = csPlaneMove;

	m_Znear = 0.2f;
	m_Zfar = 1500.f;
    m_HPB.set(0,0,0);
    m_Position.set(0,0,0);
    m_Target.set(0,0,0);
    m_CamMat.identity();

	m_FlySpeed = 5.f;
    m_FlyAltitude = 1.8f;

    m_bMoving=false;
}

CUI_Camera::~CUI_Camera()
{
}

void CUI_Camera::SetStyle(ECameraStyle new_style)
{
  	if (new_style==cs3DArcBall){
	    Fvector dir;
        dir.sub			(m_Target,m_Position);
        // parse heading
   	    Fvector DYaw; DYaw.set(dir.x,0.f,dir.z); DYaw.normalize_safe();
        if (DYaw.x<0)	m_HPB.x = acosf(DYaw.z);
   	    else			m_HPB.x = 2*PI-acosf(DYaw.z);
        // parse pitch
   	    dir.normalize_safe	();
        m_HPB.y			= asinf(dir.y);

        BuildCamera		();
    }
	m_Style				= new_style;
    UI->RedrawScene();
}

void CUI_Camera::Reset()
{
	m_HPB.set(0,0,0);
    m_Position.set(0,3,-10);
	SetStyle(m_Style);
    BuildCamera();
}

void CUI_Camera::Set(float h, float p, float b, float x, float y, float z)
{
	m_HPB.set(h,p,b);
    m_Position.set(x,y,z);
    BuildCamera();
}

void CUI_Camera::Set(const Fvector& hpb, const Fvector& pos)
{
	m_HPB.set(hpb);
    m_Position.set(pos);
    BuildCamera();
}

void CUI_Camera::BuildCamera()
{
}

void CUI_Camera::SetDepth(float _far, bool bForcedUpdate)
{
    if (m_Zfar!=_far)	{m_Zfar=_far; UI->Resize(bForcedUpdate);}
}

void CUI_Camera::SetViewport(float _near, float _far, float _fov)
{
}

void CUI_Camera::SetSensitivity(float sm, float sr)
{
    m_SM = 0.2f*(sm*sm);
    m_SR = 0.02f*(sr*sr);
}

static const Fvector down_dir={0.f,-1.f,0.f};

void CUI_Camera::Update(float dt)
{
}

void CUI_Camera::Pan(float dx, float dz)
{
    Fvector vmove;
    vmove.set( m_CamMat.k );  vmove.y = 0;
    vmove.normalize_safe();
    vmove.mul( dz*-m_SM );
    m_Position.add( vmove );

    vmove.set( m_CamMat.i );  vmove.y = 0;
    vmove.normalize_safe();
    vmove.mul( dx*m_SM );
    m_Position.add( vmove );

    BuildCamera();
}

void CUI_Camera::Scale(float dy)
{
    Fvector vmove;
    vmove.set( 0.f, dy, 0.f );
    vmove.y *= -m_SM;
    m_Position.add( vmove );

    BuildCamera();
}

void CUI_Camera::Rotate(float dx, float dy)
{
    BuildCamera();
}

void CUI_Camera::MouseRayFromPoint( Fvector& start, Fvector& direction, const Ivector2& point )
{
}

void CUI_Camera::ZoomExtents(const Fbox& bb)
{
}

