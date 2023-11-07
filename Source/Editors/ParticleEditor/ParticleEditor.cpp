// ParticleEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include "resources\splash.h"

XREPROPS_API extern bool bIsActorEditor;
ECORE_API extern bool bIsLevelEditor;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    bIsActorEditor = false;
    bIsLevelEditor = false;

    if (strstr(GetCommandLine(), "-nosplash") == nullptr)
    {
        constexpr bool topmost = false;
        splash::show(topmost);
    }
    splash::update_progress(1);

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
    splash::update_progress(5);
    Core._initialize("Patricle_Editor", ELogCallback, 1, FSName, true);

    splash::update_progress(39);
    PTools = xr_new<CParticleTool>();
    Tools  = PTools;

    splash::update_progress(5);
    UI = xr_new<CParticleMain>();
    UI->RegisterCommands();

    splash::update_progress(20);
    UIMainForm* MainForm = xr_new<UIMainForm>();
    ::MainForm           = MainForm;
    UI->Push(MainForm, false);
    splash::update_progress(29);

    splash::update_progress(1);
    while (MainForm->Frame()) {}

    xr_delete(MainForm);
    Core._destroy();
    splash::hide();
    return 0;
}
