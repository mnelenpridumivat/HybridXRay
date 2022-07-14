//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "UI_ToolsCustom.h"
#include "EditObject.h"
#include "EditMesh.h"
#include "ui_main.h"           
#include "..\xrEngine\motion.h"
#include "..\xrEngine\bone.h"
#include "library.h"
#include "..\xrEngine\fmesh.h"
#include "d3dutils.h"

//------------------------------------------------------------------------------
CToolCustom* Tools=0;
//------------------------------------------------------------------------------
#define CHECK_SNAP(R,A,C){ R+=A; if(fabsf(R)>=C){ A=snapto(R,C); R=0; }else{A=0;}}

CToolCustom::CToolCustom()
{
    m_bReady			= false;
    m_Action			= etaSelect;
    m_Settings.assign	(etfNormalAlign|etfGSnap|etfOSnap|etfMTSnap|etfVSnap|etfASnap|etfMSnap);
    fFogness			= 0.9f;
    dwFogColor			= 0xffffffff;
    m_axis_xform		= Fidentity;
}
//---------------------------------------------------------------------------

CToolCustom::~CToolCustom()
{
}
//---------------------------------------------------------------------------

bool CToolCustom::OnCreate()
{
    m_bReady 		= true;

	SetAction		(etaSelect);
    return true;
}

void CToolCustom::OnDestroy()
{
	VERIFY					(m_bReady);
    m_bReady				= false;
}
//---------------------------------------------------------------------------

void CToolCustom::SetAction(ETAction action)
{
	switch(action)
    {
    case etaSelect: 
    	m_bHiddenMode	= false; 
        break;
    case etaAdd:
    	m_bHiddenMode	= true; 
        break;
    }
    m_Action = action;
  /*  switch(m_Action)
    {
    case etaSelect:  
    	UI->GetD3DWindow()->Cursor = crCross;     
    break;
    case etaAdd:     
    	UI->GetD3DWindow()->Cursor = crArrow;     
    break;
    case etaMove:
    	if(!EPrefs->tools_show_move_axis)    
        	UI->GetD3DWindow()->Cursor = crSizeAll;   
        else
        	UI->GetD3DWindow()->Cursor = crHandPoint;   
        
    break;
    case etaRotate:  
    	UI->GetD3DWindow()->Cursor = crSizeWE;    
    break;
    case etaScale:   
    	UI->GetD3DWindow()->Cursor = crVSplit;    
    break;
    default:         
    	UI->GetD3DWindow()->Cursor = crHelp;
    }*/

 
    UI->RedrawScene();
    ExecCommand(COMMAND_REFRESH_UI_BAR);
}


void CToolCustom::SetSettings(u32 mask, BOOL val)
{
	m_Settings.set(mask,val);
    UI->RedrawScene();
    ExecCommand(COMMAND_REFRESH_UI_BAR);
}

void CToolCustom::GetCurrentFog(u32& fog_color, float& s_fog, float& e_fog)
{
    fog_color 			= dwFogColor;
}

void CToolCustom::RenderEnvironment()
{
}

void CToolCustom::Clear()
{
	ClearDebugDraw		();
}

void CToolCustom::Render()
{
}
//------------------------------------------------------------------------------

