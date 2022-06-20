// ActorEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include <iostream>
#include <string>

// argv[1] - mode
// argv[2] - object
// argv[3] - ogf / omf / skls
// argv[4] - data

extern ECORE_API BOOL g_force16BitTransformQuant;
extern ECORE_API BOOL g_forceNoCompressTransformQuant;

enum EExportFlags
{
    exf16Bit            = (1<<0),	
    exfNoCompress		= (1<<1),
    exfMakeProgressive	= (1<<2),	
    exfOptimizeSurfaces	= (1<<3),	
    exfDbgWindow    	= (1<<4),	
};

int main(int argc, char** argv)
{
    int ret_code = 0;

    Core._initialize("Actor", ELogCallback,1, "",true);

    Tools = xr_new<CActorTools>();
    ATools = (CActorTools*)Tools;

    for (int i = 0; i < argc; i++)
        std::cout << i << ". {" << argv[i] << "}" << std::endl;

    std::cout << "argc " << argc << std::endl;

    std::cout << "Import object" << std::endl;
    ATools->Load(argv[2]);

    int flags = atoi(argv[4]);

    if (flags & exf16Bit)
        g_force16BitTransformQuant = true;
    else if (flags & exfNoCompress)
        g_forceNoCompressTransformQuant = true;

    if (flags & exfMakeProgressive)
        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoProgressive, TRUE);

    if (flags & exfOptimizeSurfaces)
        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoOptimizeSurf, TRUE);

    switch (atoi(argv[1]))
    {
        case 0: // OGF
        {
            std::cout << "Export OGF" << std::endl;
            if (!ATools->ExportOGF(argv[3]))
                ret_code = -1;
        }break;
        case 1: // OMF
        {
            std::cout << "Export OMF" << std::endl;
            if (!ATools->ExportOMF(argv[3]))
                ret_code = -1;
        }break;
        case 2: // Generate shape
        {
            xr_vector<CEditableObject::ShapeEditType> vec;

            for (int i = 5; i < argc; i++)
            {
                CEditableObject::ShapeEditType shape;
                std::string first = argv[i];
                first.erase(first.find_first_of("-"), first.size());
                shape.bone_id = atoi(first.c_str());

                std::string second = argv[i];
                second.erase(second.find_last_of("-"), second.size());
                second.erase(0, first.size() + 1);
                shape.bone_type = atoi(second.c_str());

                std::string third = argv[i];
                third.erase(0, third.find_last_of("-") + 1);
                shape.bone_flags.assign(atoi(third.c_str()));

                vec.push_back(shape);
            }

            if (!ATools->CurrentObject()->ChangeBoneShapeTypes(vec) || !ATools->CurrentObject()->GenerateBoneShape(false) || !ATools->Save(argv[2]))
                ret_code = -1;
        }break;
        case 3: // Delete motions
        {
            ATools->CurrentObject()->ClearSMotions();
            if (!ATools->Save(argv[2]))
                ret_code = -1;

            Core._destroy();
            return ret_code;
        }break;
        case 4: // Load motions
        {
            if (!ATools->AppendMotion(argv[3]) || !ATools->Save(argv[2]))
                ret_code = -1;

            Core._destroy();
            return ret_code;
        }break;
        case 5: // Save motions
        {
            if (!ATools->SaveMotions(argv[3], false))
                ret_code = -1;
        }break;
        case 6: // Load bones
        {
            if (!ATools->LoadBoneData(argv[3]) || !ATools->Save(argv[2]))
                ret_code = -1;

            Core._destroy();
            return ret_code;
        }break;
        case 7:  // Save bones
        {
            if (!ATools->SaveBoneData(argv[3]))
                ret_code = -1;
        }break;
        case 8:  // Export obj
        {
            if (!ATools->ExportOBJ(argv[3]))
                ret_code = -1;

            Core._destroy();
            return ret_code;
        }break;
        case 9:  // Export DM
        {
            if (!ATools->ExportDM(argv[3]))
                ret_code = -1;

            Core._destroy();
            return ret_code;
        }break;
    }

    Core._destroy();

    if (flags & exfDbgWindow)
        system("pause");

    return ret_code;
}
