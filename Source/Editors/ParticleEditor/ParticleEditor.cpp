// ParticleEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include "resources\splash.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    if (strstr(GetCommandLine(), "-nosplash") == nullptr)
    {
        constexpr bool topmost = false;
        splash::show(topmost);
    }

    if (!IsDebuggerPresent())
        Debug._initialize(false);

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
    Core._initialize("Patricle_Editor", ELogCallback, 1, FSName, true);

    PTools = xr_new<CParticleTool>();
    Tools  = PTools;

    UI = xr_new<CParticleMain>();
    UI->RegisterCommands();

    UIMainForm* MainForm = xr_new<UIMainForm>();
    ::MainForm           = MainForm;
    UI->Push(MainForm, false);

    while (MainForm->Frame()) {}

    xr_delete(MainForm);
    Core._destroy();
    splash::hide();
    return 0;
}
