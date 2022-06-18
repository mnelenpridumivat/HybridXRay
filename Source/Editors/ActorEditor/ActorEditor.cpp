// ActorEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Core._initialize("Actor", ELogCallback,1, "",true);

    Tools = xr_new<CActorTools>();
    ATools = (CActorTools*)Tools;

    ATools->Load("H:\\jmir.object");
 //   ATools->AppendMotion("H:\\jmir.skls");
    ATools->ExportOGF("H:\\jmir.ogf");
   // ATools->ExportOMF("H:\\jmir.omf");

    Core._destroy();
    return 0;
}
