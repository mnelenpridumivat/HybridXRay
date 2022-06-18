// ActorEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
   // if (!IsDebuggerPresent()) 
        //Debug._initialize(false);
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
    Core._initialize("Actor", ELogCallback,1, FSName,true);

    Tools = xr_new<CActorTools>();
    ATools = (CActorTools*)Tools;

    ATools->Load2("H:\\jmir.object");

    Core._destroy();
    return 0;
}
