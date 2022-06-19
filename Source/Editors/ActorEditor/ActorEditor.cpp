// ActorEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include <iostream>

int main(int argc, char** argv)
{
    Core._initialize("Actor", ELogCallback,1, "",true);

    Tools = xr_new<CActorTools>();
    ATools = (CActorTools*)Tools;

    Msg("%s", argv[1]);
    Msg("%s", argv[2]);

    std::cout << argv[0] << std::endl << argv[1] << std::endl << argv[2] << std::endl;
    //system("pause");

    ATools->Load(argv[1]);
    //   ATools->AppendMotion("H:\\jmir.skls");
    ATools->ExportOGF(argv[2]);
    // ATools->ExportOMF("H:\\jmir.omf");

    Core._destroy();

    system("pause");
    return 0;
}
