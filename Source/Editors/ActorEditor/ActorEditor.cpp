// ActorEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>    

// argv[1] - mode
// argv[2] - object
// argv[3] - ogf / omf / skls || object scale
// argv[4] - data
// argv[5] - object scale

extern ECORE_API BOOL g_force16BitTransformQuant;
extern ECORE_API BOOL g_forceFloatTransformQuant;
extern ECORE_API float g_EpsSkelPositionDelta;

int iReaderPos = 0;

xr_vector<CEditableObject::ShapeEditType> LoadShapes(char** args, int count)
{
    xr_vector<CEditableObject::ShapeEditType> vec;

    for (int i = 0; i < count; i++)
    {
        CEditableObject::ShapeEditType shape;
        std::string first = args[iReaderPos];
        first.erase(first.find_first_of("-"), first.size());
        shape.bone_id = atoi(first.c_str());

        std::string second = args[iReaderPos];
        second.erase(second.find_last_of("-"), second.size());
        second.erase(0, first.size() + 1);
        shape.bone_type = atoi(second.c_str());

        std::string third = args[iReaderPos];
        third.erase(0, third.find_last_of("-") + 1);
        shape.bone_flags.assign(atoi(third.c_str()));

        vec.push_back(shape);

        iReaderPos++;
    }

    return vec;
}

xr_vector<CEditableObject::SurfaceParams> LoadSurfaces(char** args, int count)
{
    xr_vector<CEditableObject::SurfaceParams> vec;

    u8 reader = 0;
    int counter = 0;
    CEditableObject::SurfaceParams surface;

    for (int i = 0; i < int(count * 4); i++)
    {
        switch (reader)
        {
        case 0:
            surface.flags = atoi(args[iReaderPos]);
            break;
        case 1:
            surface.texture = args[iReaderPos];
            break;
        case 2:
            surface.shader = args[iReaderPos];
            break;
        }

        reader++;

        if (reader >= 3) // Конец считки, записываем сюрфейс
        {
            reader = 0;
            counter++;
            vec.push_back(surface);
        }

        iReaderPos++;

        if (counter > count - 1)
            break;
    }

    return vec;
}

xr_vector<CActorTools::BatchFiles> LoadBatchFiles(char** args, int count)
{
    xr_vector<CActorTools::BatchFiles> batch_files;
    if (count > 0) // Режим папок
    {
        for (int i = 0; i < count; i++)
        {
            CActorTools::BatchFiles file;
            file.source_folder = args[iReaderPos]; iReaderPos++;
            int cnt = atoi(args[iReaderPos]); iReaderPos++;

            for (int i = 0; i < cnt; i++)
            {
                file.files.push_back(args[iReaderPos]); iReaderPos++;
            }

            batch_files.push_back(file);
        }
    }
    else // Режим файлов
    {
        int cnt = atoi(args[iReaderPos]); iReaderPos++;

        CActorTools::BatchFiles file;
        file.source_folder = "null";
        for (int i = 0; i < cnt; i++)
        {
            file.files.push_back(args[iReaderPos]); iReaderPos++;
        }
        batch_files.push_back(file);
    }

    return batch_files;
}

xr_vector<shared_str> LoadStringVector(char** args, int count)
{
    xr_vector<shared_str> vec;

    for (int i = 0; i < count; i++)
    {
        vec.push_back(args[iReaderPos]);
        iReaderPos++;
    }

    return vec;
}

int main(int argc, char** argv)
{
    unsigned int start_time = clock();
    Core._initialize("Actor", ELogCallback,1, "",true, IsDebuggerPresent() || (atoi(argv[4]) & exfDbgWindow));
    unsigned int end_time = clock();
    double seconds = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    Msg("Core init time: %f", seconds);

    std::cout << "[Arg debugger]" << std::endl;
    std::cout << "Arg count: " << argc << std::endl;
    int size = 0;
    for (int i = 0; i < argc; i++)
    {
        shared_str arg = argv[i];
        size += arg.size();
    }
    std::cout << "Arg size: " << size << std::endl;

    // Program params
    int ret_code = 0;
    int mode;
    shared_str object_path;
    shared_str second_file_path;
    int flags = 0;
    float scale = 1.0f;
    xr_vector<CEditableObject::ShapeEditType> pShapes;
    xr_vector<CEditableObject::SurfaceParams> pSurfaces;
    xr_vector<shared_str> pLoadedAnims;
    float lod_quality = 1.0f;
    int lod_flags = 0;
    shared_str lod_path = "";
    xr_vector<shared_str> pMotionRefs;
    xr_vector<CActorTools::BatchFiles> pBatchFiles;
    shared_str batch_out = "";
    int cpp_export_mode = 0;
    shared_str custom_script = "";
    // End of program params

    if (!IsDebuggerPresent())
    {
        // Program params
        mode = atoi(argv[1]);
        object_path = argv[2];
        second_file_path = argv[3];
        flags = atoi(argv[4]);
        scale = atof(argv[5]);
        int shapes_count = atoi(argv[6]);
        int surfaces_count = atoi(argv[7]);
        int loaded_skls_count = atoi(argv[8]);
        iReaderPos = 9;
        pShapes = LoadShapes(argv, shapes_count);
        pSurfaces = LoadSurfaces(argv, surfaces_count);
        pLoadedAnims = LoadStringVector(argv, loaded_skls_count);
        lod_quality = atof(argv[iReaderPos]); iReaderPos++;
        lod_flags = atoi(argv[iReaderPos]); iReaderPos++;
        lod_path = argv[iReaderPos]; iReaderPos++;
        int motion_refs_count = atoi(argv[iReaderPos]); iReaderPos++;
        pMotionRefs = LoadStringVector(argv, motion_refs_count);
        int batch_files_count = atoi(argv[iReaderPos]); iReaderPos++;
        pBatchFiles = LoadBatchFiles(argv, batch_files_count);
        batch_out = argv[iReaderPos]; iReaderPos++;
        cpp_export_mode = atoi(argv[iReaderPos]); iReaderPos++;
        custom_script = argv[iReaderPos]; iReaderPos++;
        // End of program params
    }
    else
    {
        // Program params
        mode = ExportOGF;
        object_path = "G:\\123\\ak_no_bones.object";
        second_file_path = "G:\\123\\export\\ak_no_bones.ogf";
        flags = exfHQGeometryPlus;
        custom_script = "G:\\projects\\ValeroK\\xrExportTool\\bin\\x64\\Release\\scripts\\assign_static_to_wpn_body.script";
        // End of program params
    }

    std::string line;
    std::string userdata_path = object_path.c_str();
    xr_string userdata = "";
    userdata_path += "_temp.userdata";

    std::ifstream fuserdata(userdata_path);

    if (fuserdata.is_open())
    {
        while (getline(fuserdata, line))
        {
            userdata += line.c_str();
            userdata += "\r\n";
        }

        if (userdata.size() > 2)
        {
            userdata.pop_back();
            userdata.pop_back();
        }
    }
    fuserdata.close();

    Tools = xr_new<CActorTools>();
    ATools = (CActorTools*)Tools;

    if (mode != BatchLtx && mode != BatchDialogOGF && mode != BatchDialogOMF)
    {
        std::cout << "Import object" << std::endl;
        ATools->LoadScale(object_path.c_str(), scale, (flags & exfScaleCenterMass));

        ATools->CurrentObject()->ChangeSurfaceFlags(pSurfaces);
        ATools->CurrentObject()->ChangeBoneShapeTypes(pShapes);
        ATools->CurrentObject()->m_LODs = lod_path;
        ATools->CurrentObject()->GetClassScript() = userdata.c_str();
        ATools->CurrentObject()->m_EditorScript = custom_script;

        if (ATools->CurrentObject()->SMotionCount() == 0)
        {
            if (!IsDebuggerPresent())
                ATools->CurrentObject()->m_SMotionRefs = pMotionRefs;
        }
        else
            ATools->CurrentObject()->m_SMotionRefs.clear();

        if (!ATools->BonePartsExist() && mode != 9)
        {
            ATools->ToDefaultBoneParts(ATools->CurrentObject());
            std::cout << "Can't find bone parts, reset to default." << std::endl;
        }

        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoProgressive, (flags & exfMakeProgressive));
        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoStripify, (flags & exfMakeStripify));
        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoOptimizeSurf, (flags & exfOptimizeSurfaces));
        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoHQExportPlus, (flags & exfHQGeometryPlus));
        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoNormals, (flags & exfSplitNormals));
        ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoLod, (mode == GenerateLod));
        ATools->CurrentObject()->InitScript();
    }

    if (flags & exfHQGeometryPlus)
        g_EpsSkelPositionDelta = EPS_S;

    if (flags & exf16Bit)
        g_force16BitTransformQuant = true;
    else if (flags & exfNoCompress)
        g_forceFloatTransformQuant = true;

    switch (mode)
    {
        case ExportOGF:
        {
            std::cout << "Export OGF" << std::endl;
            if (!ATools->ExportOGF(second_file_path.c_str()))
                ret_code = -1;
        }break;
        case ExportOMF:
        {
            std::cout << "Export OMF" << std::endl;
            if (!ATools->ExportOMF(second_file_path.c_str()))
                ret_code = -1;
        }break;
        case GenerateShape:
        {
            if (!ATools->CurrentObject()->GenerateBoneShape(false) || !ATools->Save(object_path.c_str()))
                ret_code = -1;
        }break;
        case DeleteMotions:
        {
            ATools->CurrentObject()->ClearSMotions();
            if (!ATools->Save(object_path.c_str()))
                ret_code = -1;
        }break;
        case LoadMotions:
        {
            bool res = true;
            for (int i = 0; i < pLoadedAnims.size(); i++)
            {
                if (!ATools->AppendMotion(pLoadedAnims[i].c_str()))
                    res = false;
            }
            if (res)
                res = ATools->Save(object_path.c_str());

            if (!res)
                ret_code = -1;
        }break;
        case SaveSklsMotions:
        {
            if (!ATools->SaveMotions(second_file_path.c_str(), false))
                ret_code = -1;
        }break;
        case LoadBones:
        {
            if (!ATools->LoadBoneData(second_file_path.c_str()) || !ATools->Save(object_path.c_str()))
                ret_code = -1;
        }break;
        case SaveBones:
        {
            if (!ATools->SaveBoneData(second_file_path.c_str()))
                ret_code = -1;
        }break;
        case ExportOBJ:
        {
            if (!ATools->ExportOBJ(second_file_path.c_str()))
                ret_code = -1;
        }break;
        case ExportDM:
        {
            if (!ATools->ExportDM(second_file_path.c_str()))
                ret_code = -1;
        }break;
        case SaveObject:
        {
            if (!ATools->Save(object_path.c_str()))
                ret_code = -1;
        }break;
        case LoadBoneParts:
        {
            if (!ATools->LoadBoneParts(second_file_path.c_str()) || !ATools->Save(object_path.c_str()))
                ret_code = -1;
        }break;
        case SaveBoneParts:
        {
            if (!ATools->SaveBoneParts(second_file_path.c_str()))
                ret_code = -1;
        }break;
        case ToDefaultBoneParts:
        {
            if (!ATools->ToDefaultBoneParts(ATools->CurrentObject()) || !ATools->Save(object_path.c_str()))
                ret_code = -1;
        }break;
        case SaveSklMotions:
        {
            if (!ATools->SaveMotions(second_file_path.c_str(), true))
                ret_code = -1;
        }break;
        case GenerateLod:
        {
            Msg("Quality: %f", lod_quality);
            float lq = 0.01f;
            float hq = 0.002f;
            float quality = lq + (lod_quality / 1.0f * (hq - lq));
            g_EpsSkelPositionDelta = quality;
            ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoOptimizeSurf, TRUE);
            ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoHQExportPlus, FALSE);
            ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoProgressive, (lod_flags & exfMakeProgressive));
            ATools->CurrentObject()->m_objectFlags.set(CEditableObject::eoStripify, FALSE); // Крашит
            std::cout << "Export Lod" << std::endl;
            Msg("Lod quality: %f", g_EpsSkelPositionDelta);
            if (!ATools->ExportOGF(second_file_path.c_str()))
                ret_code = -1;
        }break;
        case SaveCpp:
        {
            if (!ATools->ExportCPP(second_file_path.c_str(), cpp_export_mode))
                ret_code = -1;
        }break;
        case BatchLtx:
        {
            if (!ATools->BatchConvert(second_file_path.c_str(), flags, custom_script))
                ret_code = -1;
        }break;
        case BatchDialogOGF:
        {
            if (!ATools->BatchConvertDialogOGF(pBatchFiles, batch_out, flags, custom_script))
                ret_code = -1;
        }break;
        case BatchDialogOMF:
        {
            if (!ATools->BatchConvertDialogOMF(pBatchFiles, batch_out, flags, custom_script))
                ret_code = -1;
        }break;
    }

    Core._destroy();

    if (flags & exfDbgWindow)
        system("pause");

    return ret_code;
}
