// ActorEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include <iostream>
#include <string>

// argv[1] - mode
// argv[2] - object
// argv[3] - ogf / omf / skls || object scale
// argv[4] - data
// argv[5] - object scale

extern ECORE_API BOOL g_force16BitTransformQuant;
extern ECORE_API BOOL g_forceNoCompressTransformQuant;

enum EExportFlags
{
    exf16Bit            = (1<<0),	
    exfNoCompress		= (1<<1),
    exfMakeProgressive	= (1<<2),	
    exfOptimizeSurfaces	= (1<<3),	
    exfDbgWindow    	= (1<<4),	
    exfScaleCenterMass 	= (1<<5),	
    exfHQGeometryPlus   = (1<<6),	
};

int main(int argc, char** argv)
{
    int ret_code = 0;
    int flags = atoi(argv[4]);

    Core._initialize("Actor", ELogCallback,1, "",true, (flags & exfDbgWindow));

    Tools = xr_new<CActorTools>();
    ATools = (CActorTools*)Tools;

    std::cout << "[Arg debugger]" << std::endl;
    std::cout << "Arg count: " << argc << std::endl;
    int size = 0;
    for (int i = 0; i < argc; i++)
    {
        shared_str arg = argv[i];
        size += arg.size();
    }

    std::cout << "Arg size: " << size << std::endl;
    std::cout << "Args: " << std::endl;

    for (int i = 0; i < argc; i++)
        std::cout << i << ". {" << argv[i] << "}" << std::endl;

    std::cout << "Import object" << std::endl;
    ATools->LoadScale(argv[2], atof((atoi(argv[1]) == 2) ? argv[3] : argv[5]), (flags & exfScaleCenterMass));

    if (!ATools->BonePartsExist())
    {
        ATools->ToDefaultBoneParts();
        std::cout << "Can't find bone parts, reset to default." << std::endl;
    }

    if (flags & exf16Bit)
        g_force16BitTransformQuant = true;
    else if (flags & exfNoCompress)
        g_forceNoCompressTransformQuant = true;

    if (flags & exfMakeProgressive)
        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoProgressive, TRUE);

    if (flags & exfOptimizeSurfaces)
        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoOptimizeSurf, TRUE);

    if (flags & exfHQGeometryPlus)
        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoHQExportPlus, TRUE);

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
        }break;
        case 4: // Load motions
        {
            bool res = true;
            for (int i = 6; i < argc; i++)
            {
                if (!ATools->AppendMotion(argv[i]))
                    res = false;
            }
            if (res)
                res = ATools->Save(argv[2]);

            if (!res)
                ret_code = -1;
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
        }break;
        case 9:  // Export DM
        {
            if (!ATools->ExportDM(argv[3]))
                ret_code = -1;
        }break;
        case 10:  // Save object
        {
            if (!ATools->Save(argv[2]))
                ret_code = -1;
        }break;
        case 11:  // Load bone parts
        {
            if (!ATools->LoadBoneParts(argv[3]) || !ATools->Save(argv[2]))
                ret_code = -1;
        }break;
        case 12:  // Save bone parts
        {
            if (!ATools->SaveBoneParts(argv[3]))
                ret_code = -1;
        }break;
        case 13:  // To default bone parts
        {
            if (!ATools->ToDefaultBoneParts() || !ATools->Save(argv[2]))
                ret_code = -1;
        }break;
        case 14: // Save motions
        {
            if (!ATools->SaveMotions(argv[3], true))
                ret_code = -1;
        }break;
    }

    Core._destroy();

    if (flags & exfDbgWindow)
        system("pause");

    return ret_code;
}
