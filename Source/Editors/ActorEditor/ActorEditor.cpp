// ActorEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include <iostream>

// argv[1] - mode
// argv[2] - object
// argv[3] - ogf / omf
// argv[4] - skls if exist

int main(int argc, char** argv)
{
    Core._initialize("Actor", ELogCallback,1, "",true);

    Tools = xr_new<CActorTools>();
    ATools = (CActorTools*)Tools;

    for (int i = 0; i < argc; i++)
        std::cout << i << ". {" << argv[i] << "}" << std::endl;

    std::cout << "argc " << argc << std::endl;

    std::cout << "Import object" << std::endl;
    ATools->Load(argv[2]);

    switch (atoi(argv[1]))
    {
        case 0: // OGF
        {
            std::cout << "Export OGF" << std::endl;
            //ATools->CurrentObject()->GenerateBoneShapeTest(false);
            ATools->ExportOGF(argv[3]);
        }break;
        case 1: // OMF
        {
            std::cout << "Export OMF" << std::endl;
            ATools->ExportOMF(argv[3]);
        }break;
        case 2: // Has anims check
        {
            int cnt = ATools->CurrentObject()->SMotionCount();
            Core._destroy();

            return cnt;
        }break;
        case 3: // Delete motions
        {
            ATools->CurrentObject()->ClearSMotions();
            ATools->Save(argv[2]);
        }break;
        case 4: // Load motions
        {
            ATools->AppendMotion(argv[3]);
            ATools->Save(argv[2]);
        }break;
        case 5: // Save motions
        {
            ATools->SaveMotions(argv[3], false);
        }break;
    }

    Core._destroy();
    //system("pause");
    return 0;
}
