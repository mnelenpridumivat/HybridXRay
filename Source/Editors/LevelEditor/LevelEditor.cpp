// LevelEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include "..\..\XrAPI\xrGameManager.h"
#include "Engine/XrGameManager.h"
#include "..\XrEngine\std_classes.h"
#include "..\XrEngine\IGame_Persistent.h"
#include "..\XrEngine\XR_IOConsole.h"
#include "..\XrEngine\IGame_Level.h"
#include "..\XrEngine\x_ray.h"
#include "Engine\XRayEditor.h"
#include "resources\splash.h"

XREPROPS_API extern bool bIsActorEditor;
ECORE_API extern bool bIsLevelEditor;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    bIsActorEditor = false;
    bIsLevelEditor = true;

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
    Core._initialize("Level_Editor", ELogCallback, 1, FSName, true);

    splash::update_progress(24);
    LTools = xr_new<CLevelTool>();
    Tools  = LTools;

    splash::update_progress(5);
    LUI = xr_new<CLevelMain>();
    UI  = LUI;
    UI->RegisterCommands();

    splash::update_progress(15);
    Scene                = xr_new<EScene>();
    EditorScene          = Scene;
    UIMainForm* MainForm = xr_new<UIMainForm>();
    pApp                 = xr_new<XRayEditor>();
    g_XrGameManager      = xr_new<XrGameManager>();
    g_SEFactoryManager   = xr_new<XrSEFactoryManager>();

    splash::update_progress(24);
    g_pGamePersistent = (IGame_Persistent*)g_XrGameManager->Create(CLSID_GAME_PERSISTANT);
    EDevice->seqAppStart.Process(rp_AppStart);
    Console->Execute("default_controls");
    Console->Hide();

    ::MainForm = MainForm;
    UI->Push(MainForm, false);
    splash::update_progress(25);

    splash::update_progress(1);
    while (MainForm->Frame()) {}

    xr_delete(MainForm);
    xr_delete(pApp);
    xr_delete(g_XrGameManager);
    xr_delete(g_SEFactoryManager);

    Core._destroy();
    splash::hide();
    return 0;
}
