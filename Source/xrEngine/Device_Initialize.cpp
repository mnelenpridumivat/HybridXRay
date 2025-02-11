﻿#include "stdafx.h"
#include "../xrHybridXRay/resources/resource.h"
#include "dedicated_server_only.h"

#include "device.h"
#ifdef INGAME_EDITOR
#include "../xrWeatherEditor/Public/ide.hpp"
#include "engine_impl.hpp"
#endif   // #ifdef INGAME_EDITOR

extern LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#ifdef INGAME_EDITOR
void CRenderDevice::initialize_editor()
{
    m_editor_module = LoadLibrary("xrWeatherEditor.dll");
    if (!m_editor_module)
    {
        Msg("! cannot load library \"editor.dll\"");
        return;
    }

    m_editor_initialize = (initialize_function_ptr)GetProcAddress(m_editor_module, "initialize");
    VERIFY(m_editor_initialize);

    m_editor_finalize = (finalize_function_ptr)GetProcAddress(m_editor_module, "finalize");
    VERIFY(m_editor_finalize);

    m_engine = xr_new<engine_impl>();
    m_editor_initialize(m_editor, m_engine);
    VERIFY(m_editor);

    m_hWnd = m_editor->view_handle();
    VERIFY(m_hWnd != INVALID_HANDLE_VALUE);
}
#endif   // #ifdef INGAME_EDITOR

void CRenderDevice::Initialize()
{
    Log("Initializing Engine...");
    TimerGlobal.Start();
    TimerMM.Start();

#ifdef INGAME_EDITOR
    if (strstr(Core.Params, "-weather_editor"))
        initialize_editor();
#endif   // #ifdef INGAME_EDITOR

    // Unless a substitute hWnd has been specified, create a window to render into
    if (m_hWnd == NULL)
    {
        int   _icon_ = IDI_ICON_HYBRIDXRAY;
        pcstr title  = "CoCRTeam[HybridXRay]";
        if (xrGameManager::GetGame() == EGame::COP)
        {
            _icon_ = IDI_ICON_COP;
            title  = "S.T.A.L.K.E.R.: Call of Pripyat";
        }
        else if (xrGameManager::GetGame() == EGame::CS)
        {
            _icon_ = IDI_ICON_CS;
            title  = "S.T.A.L.K.E.R.: Clear Sky";
        }
        else if (xrGameManager::GetGame() == EGame::SHOC)
        {
            _icon_ = IDI_ICON_SHOC;
            title  = "S.T.A.L.K.E.R.: Shadow of Chernobyl";
        }

        const char* wndclass  = "_XRAY_1.5";

        // Register the windows class
        HINSTANCE   hInstance = (HINSTANCE)GetModuleHandle(0);
        WNDCLASS    wndClass  = {0, WndProc, 0, 0, hInstance, LoadIcon(hInstance, MAKEINTRESOURCE(_icon_)), LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, wndclass};
        RegisterClass(&wndClass);

        // Set the window's initial style
        m_dwWindowStyle = WS_BORDER | WS_DLGFRAME;

        // Set the window's initial width
        RECT rc;
        SetRect(&rc, 0, 0, 640, 480);
        AdjustWindowRect(&rc, m_dwWindowStyle, FALSE);

        // Create the render window
        m_hWnd = CreateWindowEx(WS_EX_TOPMOST, wndclass, title, m_dwWindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, (rc.right - rc.left), (rc.bottom - rc.top), 0L, 0, hInstance, 0L);
    }

    // Save window properties
    m_dwWindowStyle = GetWindowLong(m_hWnd, GWL_STYLE);
    GetWindowRect(m_hWnd, &m_rcWindowBounds);
    GetClientRect(m_hWnd, &m_rcWindowClient);
}
