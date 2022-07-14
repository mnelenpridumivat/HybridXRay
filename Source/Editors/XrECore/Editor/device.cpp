//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop
#include <sal.h>
#include "ui_main.h"
#include "render.h"
#include "ResourceManager.h"
#include "UI_ToolsCustom.h"

CEditorRenderDevice 	*	EDevice;

extern int	rsDVB_Size;
extern int	rsDIB_Size;

ENGINE_API BOOL g_bRendering;
//---------------------------------------------------------------------------
CEditorRenderDevice::CEditorRenderDevice()
{
}

CEditorRenderDevice::~CEditorRenderDevice(){
}

//extern void Surface_Init();
#include "../../../xrAPI/xrAPI.h"
#include "../../../xrRender/Private/dxRenderFactory.h"
#include "../../../xrRender/Private/dxUIRender.h"
typedef void __cdecl ttapi_Done_func(void);
void CEditorRenderDevice::Initialize()
{
}

void CEditorRenderDevice::ShutDown()
{
//	PSLib.xrShutDown	();
}

void CEditorRenderDevice::InitTimer(){
}
//---------------------------------------------------------------------------
void CEditorRenderDevice::RenderNearer(float n){
}
void CEditorRenderDevice::ResetNearer(){
}
//---------------------------------------------------------------------------
bool CEditorRenderDevice::Create()
{
	return true;
}

//---------------------------------------------------------------------------
void CEditorRenderDevice::Destroy(){
}
//---------------------------------------------------------------------------
void CEditorRenderDevice::_SetupStates()
{
}
//---------------------------------------------------------------------------
void CEditorRenderDevice::_Create(IReader* F)
{
}

void CEditorRenderDevice::_Destroy(BOOL	bKeepTextures)
{
}

//---------------------------------------------------------------------------
void  CEditorRenderDevice::Resize(int w, int h, bool maximized)
{
}

void CEditorRenderDevice::Reset  	(bool )
{
}

bool CEditorRenderDevice::Begin	()
{
	return		TRUE;
}

//---------------------------------------------------------------------------
void CEditorRenderDevice::End()
{
}

void CEditorRenderDevice::UpdateView()
{
}

void CEditorRenderDevice::FrameMove()
{
}

void CEditorRenderDevice::DP(D3DPRIMITIVETYPE pt, ref_geom geom, u32 vBase, u32 pc)
{

}

void CEditorRenderDevice::DIP(D3DPRIMITIVETYPE pt, ref_geom geom, u32 baseV, u32 startV, u32 countV, u32 startI, u32 PC)
{
}

void CEditorRenderDevice::ReloadTextures()
{
	UI->SetStatus("Reload textures...");
	Resources->ED_UpdateTextures(0);
	UI->SetStatus("");
}

void CEditorRenderDevice::UnloadTextures()
{
}

void CEditorRenderDevice::Reset(IReader* F, BOOL bKeepTextures)
{
	CTimer tm;
    tm.Start();
	_Destroy		(bKeepTextures);
	_Create			(F);
	Msg				("*** RESET [%d ms]",tm.GetElapsed_ms());
}

void CEditorRenderDevice::time_factor(float v)
{
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void CEditorRenderDevice::CreateWindow()
{
}
void CEditorRenderDevice::DestryWindow()
{
}
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}