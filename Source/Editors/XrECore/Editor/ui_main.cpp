//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "UI_ToolsCustom.h"

#include "UI_Main.h"
#include "d3dutils.h"
#include "PSLibrary.h"
#include "..\XrETools\ETools.h"
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

}

void TUI::OnDeviceDestroy()
{
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
void TUI::PrepareRedraw()
{
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
    return true;
}

void TUI::OnDestroy()
{
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

