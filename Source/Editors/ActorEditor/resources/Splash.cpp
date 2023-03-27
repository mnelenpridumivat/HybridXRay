#include "stdafx.h"
#include "resource.h"
#include "Splash.h"
//---------------------------------------------------------------------------

HWND logoWindow = nullptr;

static INT_PTR CALLBACK LogoWndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        if (auto hBMP = LoadImage(nullptr, "Splash_AE.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE))
            SendDlgItemMessage(hw, IDC_STATIC, STM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(hBMP));
        break;
    case WM_DESTROY:
        break;
    case WM_CLOSE:
        DestroyWindow(hw);
        break;
    case WM_COMMAND:
        if (LOWORD(wp) == IDCANCEL)
            DestroyWindow(hw);
        break;
    default:
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------

namespace splash
{
    void show(const bool topmost)
    {
        if (logoWindow)
            return;

        logoWindow = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_STARTUP), nullptr, LogoWndProc);
        const HWND logoPicture = GetDlgItem(logoWindow, IDC_STATIC);
        RECT logoRect;
        GetWindowRect(logoPicture, &logoRect);
        const HWND prevWindow = topmost ? HWND_TOPMOST : HWND_NOTOPMOST;
        SetWindowPos(logoWindow, prevWindow, 0, 0, logoRect.right - logoRect.left, logoRect.bottom - logoRect.top, SWP_NOMOVE | SWP_SHOWWINDOW);
        UpdateWindow(logoWindow);
    }

    void hide()
    {
        if (logoWindow != nullptr)
        {
            DestroyWindow(logoWindow);
            logoWindow = nullptr;
        }
    }
} // namespace splash

//---------------------------------------------------------------------------
