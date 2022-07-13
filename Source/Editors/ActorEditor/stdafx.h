#pragma once
#include "..\XrECore\stdafx.h"
#include "..\XrECore\Editor\device.h"
#include "..\XrECore\Editor\UI_MainCommand.h"
#include "..\XrECore\Editor\UI_ToolsCustom.h"
#include "..\XrECore\Editor\ui_main.h"
#include "..\XrECore\Editor\EditorPreferences.h"
#include "..\XrECore\Editor\render.h"
#include "..\XrECore\Editor\UIRenderForm.h"
#include "..\XrECore\Editor\ELog.h"
#include "UITopBarForm.h"
#include "UIMainMenuForm.h"
#include "UILeftBarForm.h"
#include "UIKeyForm.h"
#include "UIBoneForm.h"
#include "UIMainForm.h"
#include "UI_ActorMain.h"
#include "UI_ActorTools.h"

enum EExportFlags
{
    exf16Bit            = (1<<0),	
    exfNoCompress		= (1<<1),
    exfMakeProgressive	= (1<<2),	
    exfOptimizeSurfaces	= (1<<3),	
    exfDbgWindow    	= (1<<4),	
    exfScaleCenterMass 	= (1<<5),	
    exfHQGeometryPlus   = (1<<6),	
    exfMakeStripify	    = (1<<7),	
    exfSplitNormals	    = (1<<8),	
};

enum EditorMode
{
    ExportOGF = 0,
    ExportOMF,
    GenerateShape,
    DeleteMotions,
    LoadMotions,
    SaveSklsMotions,
    LoadBones,
    SaveBones,
    ExportOBJ,
    ExportDM,
    SaveObject,
    LoadBoneParts,
    SaveBoneParts,
    ToDefaultBoneParts,
    SaveSklMotions,
    GenerateLod,
    SaveCpp,
    BatchLtx,
    BatchDialogOGF,
    BatchDialogOMF,
};