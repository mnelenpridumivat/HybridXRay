//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "xr_input.h"
#include "UI_ToolsCustom.h"

#include "UI_Main.h"
#include "d3dutils.h"
#include "PSLibrary.h"
#include "..\XrETools\ETools.h"
#include "gamefont.h"
#include "../XrEngine/XR_IOConsole.h"
TUI* 	UI			= 0;

TUI::TUI()
{
    m_HConsole = 0;
	UI				= this;
    m_AppClosed = false;
    m_bAppActive 	= false;
	m_bReady 		= false;
    bNeedAbort   	= false;

	m_CurrentRStart.set(0,0,0);
	m_CurrentRDir.set(0,0,0);

	m_Flags.assign	(flResize);

	m_Pivot.set		( 0, 0, 0 );

	m_MouseCaptured = false;
    m_MouseMultiClickCaptured = false;
 	m_SelectionRect = false;
    bMouseInUse		= false;

    m_bHintShowing	= false;
	m_LastHint		= "";
    m_Size.set(1280, 800);
}
//---------------------------------------------------------------------------
TUI::~TUI()
{
	VERIFY(m_ProgressItems.size()==0);
    VERIFY(m_EditorState.size()==0);
}

void TUI::OnDeviceCreate()
{
	DU_impl.OnDeviceCreate();

}

void TUI::OnDeviceDestroy()
{
	DU_impl.OnDeviceDestroy();
}

bool TUI::IsModified()
{
	return ExecCommand(COMMAND_CHECK_MODIFIED);
}
//---------------------------------------------------------------------------

void TUI::EnableSelectionRect( bool flag ){
	m_SelectionRect = flag;
	m_SelEnd.x = m_SelStart.x = 0;
	m_SelEnd.y = m_SelStart.y = 0;
}

void TUI::UpdateSelectionRect( const Ivector2& from, const Ivector2& to ){
	m_SelStart.set(from);
	m_SelEnd.set(to);
}

//----------------------------------------------------
void TUI::IR_OnMouseMove(int x, int y)
{
}
//---------------------------------------------------------------------------

void TUI::OnAppActivate()
{
}
//---------------------------------------------------------------------------

void TUI::OnAppDeactivate()
{
}
//---------------------------------------------------------------------------

bool TUI::ShowHint(const AStringVec& SS)
{
	VERIFY(m_bReady);
  /*  if (SS.size()){
        xr_string S=_ListToSequence2(SS);
        if (m_bHintShowing&&(S==m_LastHint)) return true;
        m_LastHint = S;
        m_bHintShowing = true;
        if (!m_pHintWindow){
            m_pHintWindow = xr_new<THintWindow>((TComponent*)0);
            m_pHintWindow->Brush->Color = (TColor)0x0d9F2FF;
        }
        TRect rect = m_pHintWindow->CalcHintRect(320,S,0);
        rect.Left+=m_HintPoint.x;    rect.Top+=m_HintPoint.y;
        rect.Right+=m_HintPoint.x;   rect.Bottom+=m_HintPoint.y;
        m_pHintWindow->ActivateHint(rect,S);
    }else{
    	m_bHintShowing = false;
        m_LastHint = "";
    }*/
    not_implemented();
    return m_bHintShowing;
}
//---------------------------------------------------------------------------

void TUI::HideHint()
{
	VERIFY(m_bReady);
    m_bHintShowing = false;
}
//---------------------------------------------------------------------------

void TUI::ShowHint(const xr_string& s)
{
	VERIFY			(m_bReady);
    GetCursorPos	(&m_HintPoint);
	AStringVec 		SS;
    SS.push_back	(s);
	Tools->OnShowHint(SS);
    if (!ShowHint(SS)) HideHint();
}
//---------------------------------------------------------------------------

void TUI::ShowObjectHint()
{
	/*VERIFY(m_bReady);
    if (!EPrefs->object_flags.is(epoShowHint)){
//    	if (m_bHintShowing) HideHint();
    	return;
    }
    if (EDevice->m_Camera.IsMoving()||m_MouseCaptured) return;
    if (!m_bAppActive) return;

    GetCursorPos(&m_HintPoint);
    TWinControl* ctr = FindVCLWindow(m_HintPoint);
    if (ctr!=m_D3DWindow) return;

	AStringVec SS;
	Tools->OnShowHint(SS);
    if (!ShowHint(SS)&&m_pHintWindow) HideHint();*/
}
//---------------------------------------------------------------------------
void TUI::CheckWindowPos(HWND* form)
{
	/*if (form->Left+form->Width>Screen->Width) 	form->Left	= Screen->Width-form->Width;
	if (form->Top+form->Height>Screen->Height)	form->Top 	= Screen->Height-form->Height;
	if (form->Left<0) 							form->Left	= 0;
	if (form->Top<0) 							form->Top 	= 0;*/
}
//---------------------------------------------------------------------------
#include "igame_persistent.h"
void TUI::PrepareRedraw()
{
	VERIFY(m_bReady);
	if (m_Flags.is(flResize)) 			RealResize();
// set render state
    EDevice->SetRS(D3DRS_TEXTUREFACTOR,	0xffffffff);
    // fog
    u32 fog_color;
	float fog_start, fog_end;
    Tools->GetCurrentFog	(fog_color, fog_start, fog_end);
/*
    if (0==g_pGamePersistent->Environment().GetWeather().size())
    {
        g_pGamePersistent->Environment().CurrentEnv->fog_color.set	(color_get_R(fog_color),color_get_G(fog_color),color_get_B(fog_color));
        g_pGamePersistent->Environment().CurrentEnv->fog_far		= fog_end;
        g_pGamePersistent->Environment().CurrentEnv->fog_near		= fog_start;
    }
*/    
	EDevice->SetRS( D3DRS_FOGCOLOR,		fog_color			);
	EDevice->SetRS( D3DRS_RANGEFOGENABLE,	FALSE				);
	if (HW.Caps.bTableFog)	{
		EDevice->SetRS( D3DRS_FOGTABLEMODE,	D3DFOG_LINEAR 	);
		EDevice->SetRS( D3DRS_FOGVERTEXMODE,	D3DFOG_NONE	 	);
	} else {
		EDevice->SetRS( D3DRS_FOGTABLEMODE,	D3DFOG_NONE	 	);
		EDevice->SetRS( D3DRS_FOGVERTEXMODE,	D3DFOG_LINEAR	);
	}
	EDevice->SetRS( D3DRS_FOGSTART,	*(DWORD *)(&fog_start)	);
	EDevice->SetRS( D3DRS_FOGEND,		*(DWORD *)(&fog_end)	);
    // filter
    for (u32 k=0; k<HW.Caps.raster.dwStages; k++){
        if( psDeviceFlags.is(rsFilterLinear)){
            EDevice->SetSS(k,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
            EDevice->SetSS(k,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
            EDevice->SetSS(k,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
        } else {
            EDevice->SetSS(k,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
            EDevice->SetSS(k,D3DSAMP_MINFILTER,D3DTEXF_POINT);
            EDevice->SetSS(k,D3DSAMP_MIPFILTER,D3DTEXF_POINT);
        }
    }
	// ligthing
    if (psDeviceFlags.is(rsLighting)) 	EDevice->SetRS(D3DRS_AMBIENT,0x00000000);
    else                				EDevice->SetRS(D3DRS_AMBIENT,0xFFFFFFFF);

    EDevice->SetRS			(D3DRS_FILLMODE, EDevice->dwFillMode);
    EDevice->SetRS			(D3DRS_SHADEMODE,EDevice->dwShadeMode);

    RCache.set_xform_world	(Fidentity);
}
extern ENGINE_API BOOL g_bRendering;
void TUI::Redraw()
{

}
//---------------------------------------------------------------------------
void TUI::RealResize()
{
    m_Flags.set			(flResize,FALSE);
    if(m_Size.x&& m_Size.y)
    EDevice->Resize(m_Size.x, m_Size.y,m_Size_Maximize);
    ExecCommand			(COMMAND_UPDATE_PROPERTIES);
}
void TUI::RealUpdateScene()
{
    Tools->UpdateProperties	(false);
    m_Flags.set			(flUpdateScene,FALSE);
}
void TUI::RealRedrawScene()
{

    Redraw				();         
}
void TUI::OnFrame()
{
	EDevice->FrameMove	();
    // tools on frame
    if (m_Flags.is(flUpdateScene)) RealUpdateScene();
    Tools->OnFrame		();

	// show hint
    ShowObjectHint		();
	ResetBreak			();
#if 0
	// check mail
    CheckMailslot		();
#endif
    // Progress
    ProgressDraw		();
}
bool TUI::Idle()         
{
	VERIFY(m_bReady);
   // EDevice->b_is_Active  = Application->Active;
	// input
    MSG msg;
    do
    {
        ZeroMemory(&msg, sizeof(msg));
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                UI->Quit();
            }
            continue;
        }

    } while (msg.message);
    if (m_Flags.is(flResetUI))RealResetUI();
    Sleep(1);

    OnFrame			();
    if (m_bAppActive && !m_Flags.is(flNeedQuit) && !m_AppClosed)
    RealRedrawScene();

    {
        for (u32 pit = 0; pit < Device->seqParallel.size(); pit++)
            Device->seqParallel[pit]();
        Device->seqParallel.clear_not_free();
        Device->seqFrameMT.Process(rp_Frame);
    }
    // test quit
    if (m_Flags.is(flNeedQuit))	RealQuit();
    return !m_AppClosed;
}
//---------------------------------------------------------------------------
void ResetActionToSelect()
{
    ExecCommand(COMMAND_CHANGE_ACTION, etaSelect);
}
//---------------------------------------------------------------------------

#define MIN_PANEL_HEIGHT 15


bool TUI::OnCreate()
{
// create base class
	EDevice->InitTimer();

  //  m_D3DWindow 	= w;
  //  m_D3DPanel		= p;
    EDevice->Initialize();
	// Creation

    pInput			= xr_new<CInput>(FALSE, all_device_key);

    Console = xr_new<CConsole>();
    Console->Initialize();

    UI->IR_Capture	();

    m_bReady		= true;

#if 0
    if (!CreateMailslot()) {
        ELog.DlgMsg(mtError, "Can't create mail slot.\nIt's possible two Editors started.");
        return 		false;
    }
#endif
    string_path log_path;
    if (!FS.exist(log_path,_temp_,""))
    {
        VerifyPath(log_path);
    }
    if (!FS.path_exist(_local_root_)){
    	ELog.DlgMsg	(mtError,"Undefined Editor local directory.");
        return 		false;
    }

	BeginEState		(esEditScene);
    GetRenderWidth() = 128;
    GetRenderHeight() = 128;
    RTSize.set(GetRenderWidth(), GetRenderHeight());
    EDevice->fASPECT = (float)RTSize.x / (float)RTSize.y;
    EDevice->mProject.build_projection(deg2rad(EDevice->fFOV), EDevice->fASPECT, EDevice->m_Camera.m_Znear, EDevice->m_Camera.m_Zfar);
    EDevice->m_fNearer = EDevice->mProject._43;


    RCache.set_xform_project(EDevice->mProject);
    RCache.set_xform_world(Fidentity);
    RT.create("rt_color", RTSize .x*EDevice->m_ScreenQuality, RTSize.y * EDevice->m_ScreenQuality, HW.Caps.fTarget);
    ZB.create("rt_depth", RTSize.x * EDevice->m_ScreenQuality, RTSize.y* EDevice->m_ScreenQuality, D3DFORMAT::D3DFMT_D24X8);

    return true;
}

void TUI::OnDestroy()
{
    Console->Destroy();
    xr_delete(Console);
    RT.destroy();
    ZB.destroy();

	VERIFY(m_bReady);
	m_bReady		= false;
	UI->IR_Release	();
    xr_delete		(pInput);
    EndEState		();

    EDevice->ShutDown();    
}

SPBItem* TUI::ProgressStart		(float max_val, LPCSTR text)
{
	VERIFY(m_bReady);
	SPBItem* item 				= xr_new<SPBItem>(text,"",max_val);
    m_ProgressItems.push_back	(item);
    ELog.Msg					(mtInformation,text);
    ProgressDraw				();
    if (!m_HConsole)
    {
        AllocConsole();
        m_HConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
	return item;
}
void TUI::ProgressEnd			(SPBItem*& pbi)
{
	VERIFY(m_bReady);
    if (pbi){
        PBVecIt it=std::find(m_ProgressItems.begin(),m_ProgressItems.end(),pbi); VERIFY(it!=m_ProgressItems.end());
        m_ProgressItems.erase	(it);
        xr_delete				(pbi);
        ProgressDraw			();
        if (m_ProgressItems.size() == 0)
        {
            FreeConsole();
            m_HConsole = 0;
        }
    }
}

void TUI::ProgressDraw()
{
    SPBItem* pbi = UI->ProgressLast();
    if (pbi) 
    {
        xr_string txt;
        float 		p, m;
        pbi->GetInfo(txt, p, m);
        // progress
        int val = fis_zero(m) ? 0 : (int)((p / m) * 100);
        string2048 out;
        xr_sprintf(out,"[%d%%]%s\r\n", val, txt.c_str());
        DWORD  dw;
        WriteConsole(m_HConsole, out, xr_strlen(out), &dw, NULL);
    }
}

void TUI::OnDrawUI()
{

}

void TUI::RealResetUI()
{
}

void TUI::OnStats(CGameFont* font)
{
}

void SPBItem::GetInfo			(xr_string& txt, float& p, float& m)
{
    if (info.size())txt.sprintf("%s (%s)",text.c_str(),info.c_str());
    else			txt.sprintf("%s",text.c_str());
    p				= progress;
    m				= max;
}  
void SPBItem::Inc				(LPCSTR info, bool bWarn)
{
    Info						(info,bWarn);
    Update						(progress+1.f);
}
void SPBItem::Update			(float val)
{
    progress					= val;
    UI->ProgressDraw			();
}
void SPBItem::Info				(LPCSTR text, bool bWarn)
{
	if (text&&text[0]){
    	info					= text;
        xr_string 				txt;
        float 					p,m;
        GetInfo					(txt,p,m);
	    ELog.Msg				(bWarn?mtError:mtInformation,txt.c_str());
	    UI->ProgressDraw		();
    }
}

