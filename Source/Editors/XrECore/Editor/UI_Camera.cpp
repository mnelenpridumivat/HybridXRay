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
	if (m_HPB.x>PI_MUL_2)  m_HPB.x-=PI_MUL_2;	if (m_HPB.x<-PI_MUL_2) m_HPB.x+=PI_MUL_2;
	if (m_HPB.y>PI_MUL_2)  m_HPB.y-=PI_MUL_2;	if (m_HPB.y<-PI_MUL_2) m_HPB.y+=PI_MUL_2;
	if (m_HPB.z>PI_MUL_2)  m_HPB.z-=PI_MUL_2;	if (m_HPB.z<-PI_MUL_2) m_HPB.z+=PI_MUL_2;

    if (m_Style==cs3DArcBall){
	    Fvector D;
    	D.setHP			(m_HPB.x,m_HPB.y);
		float dist = m_Position.distance_to(m_Target);
	    m_Position.mul	(D,-dist);
    	m_Position.add	(m_Target);
    }

	m_CamMat.setHPB(m_HPB.x,m_HPB.y,m_HPB.z);
    m_CamMat.translate_over(m_Position);
    UI->OutCameraPos();
    
	EDevice->vCameraPosition.set	(m_CamMat.c);
	EDevice->vCameraDirection.set	(m_CamMat.k);
	EDevice->vCameraTop.set		(m_CamMat.j);
    EDevice->vCameraRight.set		(m_CamMat.i);
}

void CUI_Camera::SetDepth(float _far, bool bForcedUpdate)
{
    if (m_Zfar!=_far)	{m_Zfar=_far; UI->Resize(bForcedUpdate);}
}

void CUI_Camera::SetViewport(float _near, float _far, float _fov)
{
    if (m_Znear!=_near)		{m_Znear=_near; UI->Resize();}
    if (m_Zfar!=_far)		{m_Zfar=_far; UI->Resize();}
    if (EDevice->fFOV!=_fov)	{EDevice->fFOV=_fov; UI->Resize();}
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
	m_HPB.x-=m_SR*dx;
	m_HPB.y-=m_SR*dy*EDevice->fASPECT;

    BuildCamera();
}

void CUI_Camera::MouseRayFromPoint( Fvector& start, Fvector& direction, const Ivector2& point )
{
	int halfwidth  = UI->GetRenderWidth()*0.5f;
	int halfheight = UI->GetRenderHeight()*0.5f;

    if (!halfwidth||!halfheight) return;

    Ivector2 point2;
    point2.set(point.x-halfwidth, halfheight-point.y);

	start.set( m_Position );

	float size_y = m_Znear * tan( deg2rad(EDevice->fFOV) * 0.5f );
	float size_x = size_y / EDevice->fASPECT;

	float r_pt = float(point2.x) * size_x / (float) halfwidth;
	float u_pt = float(point2.y) * size_y / (float) halfheight;

	direction.mul( m_CamMat.k, m_Znear );
	direction.mad( direction, m_CamMat.j, u_pt );
	direction.mad( direction, m_CamMat.i, r_pt );
	direction.normalize();
}

void CUI_Camera::ZoomExtents(const Fbox& bb)
{
	Fvector C,D;
    float R,H1,H2;
    bb.getsphere(C,R);
	D.mul(m_CamMat.k,-1);
    H1 = R/sinf(deg2rad(EDevice->fFOV)*0.5f);
    H2 = R/sinf(deg2rad(EDevice->fFOV)*0.5f/EDevice->fASPECT);
    m_Position.mad(C,D,_max(H1,H2));
	m_Target.set(C);

	BuildCamera();
/*
	eye_k - фокусное расстояние, eye_k=eye_width/2
	camera.alfa:=0;
     camera.beta:=-30*pi/180;
     camera.gama:=0;
     s:=(maxx-minx)*eye_k/eye_width*0.5*0.5;
     camera.posx:=(maxx+minx)/2;
     camera.posy:=maxy+s*tan(30*pi/180);
     camera.posz:=minz-s;
*/
}

