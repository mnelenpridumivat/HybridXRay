#include "stdafx.h"
#include "resource.h"
#include "Splash.h"
#include <CommCtrl.h>
//---------------------------------------------------------------------------

HINSTANCE hInst;                    // Указатель на экземпляр приложения.
HWND      hProgress;                // Дескриптор прогресс бара.
DWORD     IDC_TIMER;                // Номер таймера.
DWORD     nCounter;                 // Счётчик.
COLORREF  clrBg = RGB(25, 25, 0);   // Цвет фона прогресс бара.
COLORREF clrBarRed = RGB(255, 0, 0);   // Цвет индикатора хода выполнения прогресс бара.
COLORREF clrBarYellow = RGB(255, 255, 0);   // Цвет индикатора хода выполнения прогресс бара.
COLORREF clrBarGreen = RGB(0, 255, 0);   // Цвет индикатора хода выполнения прогресс бара.

HWND    hText;
HWND    hText_loading;
HWND    hText_initializing;
HWND    hText_complete;
CHAR    s_text_loading[]      = {"Loading..."};
CHAR    s_text_initializing[] = {"Core initializing..."};
CHAR    s_text_complete[]     = {"Loading is Complete!"};
HBITMAP hSplashBMP;
HWND    logoWindow = nullptr;

static INT_PTR CALLBACK LogoWndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
        case WM_PAINT:
        {
            char        buffer[64];
            RECT        rect;
            PAINTSTRUCT ps;
            HDC         hdc   = BeginPaint(hw, &ps);
            HBRUSH      brush = CreatePatternBrush(hSplashBMP);

            GetClientRect(hw, &rect);
            FillRect(hdc, &rect, brush);
            DeleteObject(brush);

            rect.top = 285;
            sprintf_s(buffer, "Counter -> %d", nCounter);
            SetTextColor(hdc, RGB(0x00, 0xFF, 0x00));
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, buffer, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_CENTER);
            EndPaint(hw, &ps);
        }
        break;
        case WM_INITDIALOG:

            hSplashBMP = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SPLASH)); //Here we have to use the executable module to load our bitmap resource

            hProgress = CreateWindowExA(
                0,
                PROGRESS_CLASS,   // Указатель на класс прогресс бара
                (LPTSTR)NULL, WS_CHILD | WS_VISIBLE /* | PBS_SMOOTH*/ | WS_BORDER,
                65,    // X
                313,   // Y
                370,   // Ширина
                13,    // Высота
                hw, (HMENU)0, hInst, NULL);
            // Устанавливаем Цвета прогресс бара.
            SendMessage(hProgress, PBM_SETBKCOLOR, 0, (LPARAM)clrBg);
            SendMessage(hProgress, PBM_SETBARCOLOR, 0, (LPARAM)clrBarRed);
            // Устанавливаем диапазон прогресс бара.
            SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
            SendMessage(hProgress, PBM_SETSTEP, (WPARAM)1, 0);
            // Устанавливаем таймер на 100 милисекунд.
            SetTimer(hw, IDC_TIMER, 100, 0);
            nCounter = 0;

            SendMessage(hText_loading, WM_SETTEXT, 0, (LPARAM)s_text_loading);
            break;
        case WM_TIMER: {
            nCounter++;
            
            // Передвигаем таймер на заданную позицию.
            SendMessage(hProgress, PBM_STEPIT, 0, 0);

            if (nCounter == 100)
                SendMessage(hProgress, PBM_SETBARCOLOR, 0, (LPARAM)clrBarRed);
            if (nCounter == 150)
                SendMessage(hProgress, PBM_SETBARCOLOR, 0, (LPARAM)clrBarYellow);
            if (nCounter == 250)
                SendMessage(hProgress, PBM_SETBARCOLOR, 0, (LPARAM)clrBarGreen);

            if (nCounter == 300)
            {
                KillTimer(hw, IDC_TIMER);
            }

            RedrawWindow(hw, NULL, NULL, RDW_INVALIDATE);
        }
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
        RECT       logoRect;
        GetWindowRect(logoPicture, &logoRect);
        const HWND prevWindow = topmost ? HWND_TOPMOST : HWND_NOTOPMOST;
        SetWindowPos(
            logoWindow, prevWindow, 0, 0, logoRect.right - logoRect.left, logoRect.bottom - logoRect.top,
            SWP_NOMOVE | SWP_SHOWWINDOW);
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
}   // namespace splash

//---------------------------------------------------------------------------
