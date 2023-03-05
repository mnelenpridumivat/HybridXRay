// ParticleEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include "resource.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    if (!IsDebuggerPresent()) Debug._initialize(false);
    const char* FSName = "fs.ltx";
    {
        if (strstr(GetCommandLine(), "-soc_14") || strstr(GetCommandLine(), "-soc_10004"))
        {
            FSName = "fs_soc.ltx";
        }
        else if (strstr(GetCommandLine(), "-soc"))
        {
            FSName = "fs_soc.ltx";
        }
        else if (strstr(GetCommandLine(), "-cs"))
        {
            FSName = "fs_cs.ltx";
        }
    }
    Core._initialize("Patricle", ELogCallback, 1, FSName, true);

    Tools = xr_new<CParticleTool>();
    PTools = (CParticleTool*)Tools;
    UI = xr_new<CParticleMain>();
    UI->RegisterCommands();

    UIMainForm* MainForm = xr_new< UIMainForm>();

    HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
    if (icon)
    {
        SendMessage(EDevice->m_hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
        SendMessage(EDevice->m_hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
    }

    ::MainForm = MainForm;
    UI->Push(MainForm, false);
    while (MainForm->Frame())
    {
    }
    xr_delete(MainForm);
    Core._destroy();
    return 0;
}
