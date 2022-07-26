//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop
#define dSINGLE
#include "animation_blend.h"
#include "..\XrECore\Editor\EditMesh.h"
#include "KinematicAnimatedDefs.h"
#include "SkeletonAnimated.h"
CActorTools*	ATools=(CActorTools*)Tools;
//------------------------------------------------------------------------------
#define CHECK_SNAP(R,A,C){ R+=A; if(fabsf(R)>=C){ A=snapto(R,C); R=0; }else{A=0;}}

ECORE_API void ShapeRotate(CBone& Bone, const Fvector& _amount);
ECORE_API void ShapeMove(CBone& Bone, const Fvector& _amount);
ECORE_API void BoneRotate(CBone& Bone, const Fvector& _axis, float angle);


void EngineModel::DeleteVisual		()
{
	DeletePhysicsShell	();
    m_pVisual	= 0;
	m_pBlend	= 0;
}

void	   EngineModel::		OnRender			()
{
    Fmatrix temp = Fmatrix();
    UpdateObjectXform(temp);
}

/*
void PreviewModel::RestoreParams(TFormStorage* s)
{          
    m_Props->RestoreParams(s);
    m_LastObjectName	= s->ReadString	("preview_name","");
    int val;
    val					= s->ReadInteger("preview_speed",0); 	m_fSpeed 	= *((float*)&val);
    val					= s->ReadInteger("preview_segment",0); 	m_fSegment	= *((float*)&val);
    m_Flags.assign		(s->ReadInteger("preview_flags",0));
    m_ScrollAxis		= (EScrollAxis)s->ReadInteger("preview_scaxis",0);
}

void PreviewModel::SaveParams(TFormStorage* s)
{
    m_Props->SaveParams(s);
    s->WriteString	("preview_name",	m_LastObjectName);
    s->WriteInteger	("preview_speed",	*((int*)&m_fSpeed));
    s->WriteInteger	("preview_segment",	*((int*)&m_fSegment));
    s->WriteInteger	("preview_flags",	m_Flags.get());
    s->WriteInteger	("preview_scaxis",	m_ScrollAxis);
}
*/
/*
void PreviewModel::OnDestroy()
{
	TProperties::DestroyForm(m_Props);
}

void PreviewModel::OnCreate()
{
    m_Props = TProperties::CreateForm("Preview prefs",0,alNone);
}

void PreviewModel::Clear()
{
    Lib.RemoveEditObject(m_pObject);
}
void PreviewModel::SelectObject()
{
    LPCSTR fn;
    if (!TfrmChoseItem::SelectItem(smObject,fn,1,m_LastObjectName.c_str())) return;
    Lib.RemoveEditObject(m_pObject);
    m_pObject = Lib.CreateEditObject(fn);
    if (!m_pObject)	ELog.DlgMsg(mtError,"Object '%s' can't find in object library.",fn);
    else			m_LastObjectName = fn;
}

xr_token		sa_token					[ ]={
	{ "+Z",		PreviewModel::saZp	},
	{ "-Z",		PreviewModel::saZn	},
	{ "+X",		PreviewModel::saXp	},
	{ "-X",		PreviewModel::saXn	},
	{ 0,		0								}
};

void PreviewModel::SetPreferences()
{
	PropItemVec items;
    PHelper().CreateFlag32		(items, 	"Scroll", 		&m_Flags, 		pmScroll);
    PHelper().CreateFloat		(items, 	"Speed (m/c)",	&m_fSpeed,		-10000.f,10000.f,0.01f,2);
    PHelper().CreateFloat		(items, 	"Segment (m)",	&m_fSegment,	-10000.f,10000.f,0.01f,2);
    PHelper().CreateToken32		(items,		"Scroll axis",	(u32*)&m_ScrollAxis,	sa_token);
	m_Props->AssignItems		(items);
    m_Props->ShowProperties		();
}
void PreviewModel::Render()
{
	if (m_pObject){
        float angle;
    	switch (m_ScrollAxis){
        case saZp: angle = 0;		break;
        case saZn: angle = PI;		break;
        case saXp: angle = PI_DIV_2;break;
        case saXn: angle =-PI_DIV_2;break;
        default: THROW;
        }
		Fmatrix R,T;
       	R.rotateY(angle);
    	T.translate(m_vPosition);
        T.mulA_43(R);
    	m_pObject->RenderSingle(T);
    }
}

void PreviewModel::Update()
{
    if (m_Flags.is(pmScroll))
    {
        m_vPosition.z += m_fSpeed*EDevice->fTimeDelta;
        if (m_vPosition.z>m_fSegment) m_vPosition.z-=m_fSegment;
    }
}

void _SynchronizeTextures()
{   
	FS_FileSet M_THUM;
    FS.file_list(M_THUM,_textures_,FS_ListFiles|FS_ClampExt,"*.thm");

    FS_FileSetIt it		= M_THUM.begin();
	FS_FileSetIt _E 	= M_THUM.end();
	for (; it!=_E; it++){
		ETextureThumbnail* THM=0;
        THM = xr_new<ETextureThumbnail>(it->name.c_str());
        STextureParams& fmt = THM->_Format();
        if (fmt.material==STextureParams::tmOrenNayar_Blin){
        	fmt.material=STextureParams::tmBlin_Phong;
	        THM->Save(0,0);
        }
	    xr_delete(THM);
    }
}

*/


CActorTools::CActorTools()
{
    m_IsPhysics = false;
    m_ChooseSkeletonBones = false;
    m_pEditObject = 0;
    m_bObjectModified = false;
    m_Flags.zero();
    m_EditMode = emObject;
    fFogness = 0.9f;
    dwFogColor = 0xffffffff;
}

CActorTools::~CActorTools()
{
}

#include "..\xrECore\editor\d3dutils.h"

const u32 color_bone_sel_color = 0xFFFFFFFF;
const u32 color_bone_norm_color = 0xFFFFFF00;
const u32 color_bone_link_color = 0xFFA0A000;
const u32 color_bone_sel_cm = 0xFFFF0000;
const u32 color_bone_norm_cm = 0xFF700000;
const float joint_size = 0.025f;

void CActorTools::Render()
{
}

void CActorTools::RenderEnvironment()
{
}

void CActorTools::OnFrame()
{
}

bool CActorTools::OnCreate()
{

    
    inherited::OnCreate();
     m_PreviewObject.OnCreate();

    // key bar

    OnDeviceCreate();
    return true;
}

void CActorTools::OnDestroy()
{
    inherited::OnDestroy();

    m_PreviewObject.OnDestroy();

    m_PreviewObject.Clear();
    m_RenderObject.Clear();
    xr_delete(m_pEditObject);
}

bool CActorTools::IfModified()
{
    if (IsModified())
    {
        int mr = ELog.DlgMsg(mtConfirmation, "The '%s' has been modified.\nDo you want to save your changes?", GetEditFileName().c_str());
        switch (mr)
        {
        case mrYes: if (!ExecCommand(COMMAND_SAVE)) return false; else { m_bObjectModified = false; }break;
        case mrNo: m_bObjectModified = false; break;
        case mrCancel: return false;
        }
    }
    return true;
}

void CActorTools::Modified()
{
    m_bObjectModified = true;
    ExecCommand(COMMAND_UPDATE_CAPTION);
}
void  CActorTools::OnBoneModified(void)
{
    Modified();
    RefreshSubProperties();
    UndoSave();
}
void CActorTools::OnItemModified(void)
{
    switch (m_EditMode) {
    case emObject:      OnObjectModified();		break;
    case emMotion: 		OnMotionDefsModified();	break;
    case emBone:      	OnBoneModified();		break;
    case emSurface:     OnObjectModified();		break;
    case emMesh:		break;
    }
}



LPCSTR CActorTools::GetInfo()
{
    return 0;
}

void CActorTools::ZoomObject(BOOL bSelOnly)
{
    if (m_pEditObject) {
        Fbox BB;
        switch (m_EditMode) {
        case emBone: {
            BoneVec lst;
            if (m_pEditObject->GetSelectedBones(lst)) {
                BB.invalidate();
                for (BoneIt b_it = lst.begin(); b_it != lst.end(); b_it++) {
                    Fvector 	C = { 0,0,0 };
                    float		r = 0.5f;
                    switch ((*b_it)->shape.type) {
                    case SBoneShape::stBox:
                        r = _max(_max((*b_it)->shape.box.m_halfsize.x, (*b_it)->shape.box.m_halfsize.y), (*b_it)->shape.box.m_halfsize.z);
                        C = (*b_it)->shape.box.m_translate;
                        break;
                    case SBoneShape::stSphere:
                        r = (*b_it)->shape.sphere.R;
                        C = (*b_it)->shape.sphere.P;
                        break;
                    case SBoneShape::stCylinder:
                        r = _max((*b_it)->shape.cylinder.m_height, (*b_it)->shape.cylinder.m_radius);
                        C = (*b_it)->shape.cylinder.m_center;
                        break;
                    }
                    (*b_it)->_LTransform().transform_tiny(C);
                    m_AVTransform.transform_tiny(C);
                    Fbox bb; 	bb.set(C, C); bb.grow(r);
                    BB.merge(bb);
                }
            }
        }break;
        default:
            BB = m_pEditObject->GetBox();
        }
        EDevice->m_Camera.ZoomExtents(BB);
    }
}

#include "../XrECore/Editor/EditObject.h"

bool CActorTools::Load(LPCSTR obj_name)
{
    Msg("Import object [%s]", obj_name);
    xr_string 		full_name;
    full_name = obj_name;

    CEditableObject* O = xr_new<CEditableObject>(obj_name);
    if (FS.exist(full_name.c_str()) && O->Load(full_name.c_str())) 
    {
        full_name += ".ogf";
        xr_delete(m_pEditObject);
        m_pEditObject = O;
        //  m_pEditObject->Optimize ();
        // delete visual
        return true;
    }
    else 
    {
        ELog.DlgMsg(mtError, "Can't load object file '%s'.", obj_name);
    }
    xr_delete(O);
    return false;
}

bool CActorTools::LoadScale(LPCSTR obj_name, float scale, bool scale_mass)
{
    Msg("Import object [%s]", obj_name);
    xr_string 		full_name;
    full_name = obj_name;

    CEditableObject* O = xr_new<CEditableObject>(obj_name);
    if (FS.exist(full_name.c_str()) && O->Load(full_name.c_str())) 
    {
        full_name += ".ogf";
        xr_delete(m_pEditObject);
        m_pEditObject = O;
        m_pEditObject->a_vScale = scale;
        m_pEditObject->a_vAdjustMass = !!scale_mass;
      //  m_pEditObject->Optimize ();
        // delete visual
        return true;
    }
    else 
    {
        ELog.DlgMsg(mtError, "Can't load object file '%s'.", obj_name);
    }
    xr_delete(O);
    return false;
}

bool CActorTools::Save(LPCSTR obj_name, bool bInternal)
{
    xr_string 		full_name;
    full_name = obj_name;
    if (m_pEditObject) {
        //EFS.MarkFile(full_name.c_str(), true);
        if (m_pEditObject->Save(full_name.c_str()))
        {
            if (!bInternal)
                m_bObjectModified = false;
            return true;
        }
    }
    return false;
}

void CActorTools::Reload()
{
    // visual part
}

void CActorTools::OnDeviceCreate()
{
    if (m_pEditObject) {
        m_pEditObject->OnDeviceCreate();
        MakePreview();
    }
}

void CActorTools::OnDeviceDestroy()
{
    if (m_pEditObject) {
        m_pEditObject->OnDeviceDestroy();
        m_RenderObject.DeleteVisual();
    }
}

void CActorTools::Clear()
{
    PhysicsStopSimulate();
    inherited::Clear();
    m_CurrentMotion = "";
    m_CurrentSlot = 0;
    // delete visuals
    if (m_pEditObject)
        m_pEditObject->DeletePhysicsShell();
    xr_delete(m_pEditObject);
    m_RenderObject.Clear();
    //	m_PreviewObject.Clear();

    m_bObjectModified = false;
    m_Flags.set(flUpdateGeometry | flUpdateMotionDefs | flUpdateMotionKeys | flReadOnlyMode, FALSE);
    m_EditMode = emObject;

    UI->RedrawScene();
}

void CActorTools::OnShowHint(AStringVec& SS)
{
}

bool CActorTools::RayPick(const Fvector& start, const Fvector& dir, float& dist, Fvector* pt, Fvector* n)
{
    if (m_PreviewObject.m_pObject)
    {
        SRayPickInfo pinf;
        if (m_PreviewObject.m_pObject->RayPick(dist, start, dir, Fidentity, &pinf))
        {
            if (pt) pt->set(pinf.pt);
            return true;
        }
        else
            return false;
    }
    else
    {
        Fvector np; np.mad(start, dir, dist);
        if ((start.y > 0) && (np.y < 0.f))
        {
            if (pt) pt->set(start);
            if (n)	n->set(0.f, 1.f, 0.f);
            return true;
        }
        else
            return false;
    }
}

void CActorTools::GetStatTime(float& a, float& b, float& c)
{
}

bool CActorTools::IsEngineMode()
{
    return false;
}

void CActorTools::SelectPreviewObject(bool bClear)
{
    if (bClear) { m_PreviewObject.Clear(); return; }
    m_PreviewObject.SelectObject();
}

void CActorTools::SetPreviewObjectPrefs()
{
    m_PreviewObject.SetPreferences();
}

void CActorTools::OnObjectModified(void)
{
    m_Flags.set(flUpdateGeometry, TRUE);
    OnGeometryModified();
}

void CActorTools::ShowClipMaker()
{
}

bool CActorTools::Import(LPCSTR initial, LPCSTR obj_name)
{
    string_path 	full_name;
    if (initial)
        FS.update_path(full_name, initial, obj_name);
    else
        strcpy(full_name, obj_name);

    CEditableObject* O = xr_new<CEditableObject>(obj_name);
    if (O->Load(full_name))
    {
        O->m_objectFlags.set(CEditableObject::eoDynamic, TRUE);
        O->m_objectFlags.set(CEditableObject::eoProgressive, TRUE);
        xr_delete(m_pEditObject);
        m_pEditObject = O;
        // delete visual
        m_RenderObject.Clear();

        UpdateProperties();
        return true;
    }
    else {
        ELog.DlgMsg(mtError, "Can't load object file '%s'.", obj_name);
    }
    xr_delete(O);

    return false;
}

bool CActorTools::ExportOGF(LPCSTR name)
{
    if (m_pEditObject && m_pEditObject->ExportOGF(name, 4)) return true;
    return false;
}

bool CActorTools::ExportOMF(LPCSTR name)
{
    if (m_pEditObject && m_pEditObject->ExportOMF(name)) return true;
    return false;
}

bool CActorTools::ExportOBJ(LPCSTR name)
{
    if (m_pEditObject && m_pEditObject->ExportOBJ(name)) return true;
    return false;
}


bool CActorTools::ExportCPP(LPCSTR name, int mode)
{
    if (m_pEditObject)
    {
        EditMeshVec& meshes = m_pEditObject->Meshes();
        string128 tmp;
        IWriter* W = FS.w_open(name);

        for (EditMeshIt m_it = meshes.begin(); m_it != meshes.end(); m_it++)
        {
            CEditableMesh* mesh = *m_it;
            mesh->GenerateVNormals(true);
            const st_Face* faces = mesh->GetFaces();
            const Fvector* verts = mesh->GetVertices();
            const Fvector* vnormals = mesh->GetVNormals();
            const Fvector* normals = mesh->GetNormals();
            sprintf(tmp, "MESH %s {", mesh->Name().c_str());
            W->w_string(tmp);
            sprintf(tmp, "\tconst size_t VERTEX_COUNT %d", mesh->GetVCount());
            W->w_string(tmp);
            sprintf(tmp, "\tconst size_t FACE_COUNT %d", mesh->GetFCount());
            W->w_string(tmp);

            if (mode == 0 || mode == 1)
            {
                W->w_string("\tconst Fvector vertices[VERTEX_COUNT] = {");
                for (u32 v_id = 0; v_id < mesh->GetVCount(); v_id++)
                {
                    sprintf(tmp, "\t\t{% 3.6f,\t% 3.6f,\t% 3.6f},", VPUSH(verts[v_id]));
                    W->w_string(tmp);
                }
                W->w_string("\t}");
            }

            if (mode == 0 || mode == 2)
            {
                W->w_string("\tconst u16 faces[FACE_COUNT*3] = {");
                for (u32 f_id = 0; f_id < mesh->GetFCount(); f_id++)
                {
                    sprintf(tmp, "\t\t%-d,\t\t%-d,\t\t%-d,", faces[f_id].pv[0].pindex, faces[f_id].pv[1].pindex, faces[f_id].pv[2].pindex);
                    W->w_string(tmp);
                }
                W->w_string("\t}");
            }

            if (mode == 0 || mode == 3)
            {
                W->w_string("\tconst Fvector vnormals[FACE_COUNT*3] = {");
                for (u32 vn_id = 0; vn_id < mesh->GetFCount() * 3; vn_id++)
                {
                    sprintf(tmp, "\t\t{% 3.6f,\t% 3.6f,\t% 3.6f},", VPUSH(vnormals[vn_id]));
                    W->w_string(tmp);
                }
                W->w_string("\t}");
            }

            if ((mode == 0 || mode == 4) && normals)
            {
                W->w_string("\tconst Fvector normals[FACE_COUNT*3] = {");
                for (u32 n_id = 0; n_id < mesh->GetFCount() * 3; n_id++)
                {
                    sprintf(tmp, "\t\t{% 3.6f,\t% 3.6f,\t% 3.6f},", VPUSH(normals[n_id]));
                    W->w_string(tmp);
                }
                W->w_string("\t}");
            }

            W->w_string("}");
        }
        FS.w_close(W);
        return true;
    }
    return false;
}

#include "../xrECore/Editor/EDetailModel.h"
bool CActorTools::ExportDM(LPCSTR name)
{
    if (m_pEditObject) {
        EDetail DM;
        if (!DM.Update(m_pEditObject->GetName())) return false;
        DM.Export(name);
        return true;
    }
    return false;
}

void CActorTools::ChangeSurfaceFlags(xr_vector<CEditableObject::SurfaceParams> params)
{
    m_pEditObject->ChangeSurfaceFlags(params);
}

void CActorTools::WorldMotionRotate(const Fvector& R)
{
    R_ASSERT(m_pEditObject && (!m_CurrentMotion.empty()));
    CSMotion* M = m_pEditObject->FindSMotionByName(m_CurrentMotion.c_str());
    int rootId = m_pEditObject->GetRootBoneID();
    //M->WorldRotate(rootId, R.y, R.x, R.z);
    OnMotionKeysModified();
}



void CActorTools::SetCurrentMotion(LPCSTR name, u16 slot)
{
    if (m_pEditObject)
    {
        if ((m_CurrentMotion != name) || (m_CurrentSlot != slot))
        {
            m_CurrentMotion = name;
            m_CurrentSlot = slot;
            CSMotion* M = m_pEditObject->FindSMotionByName(name);
            if (M)
            {
                m_pEditObject->SetActiveSMotion(M);
            }
            PlayMotion();
        }
    }
}

CSMotion* CActorTools::GetCurrentMotion()
{
    return m_pEditObject ? m_pEditObject->FindSMotionByName(m_CurrentMotion.c_str()) : 0;
}

CSMotion* CActorTools::FindMotion(LPCSTR name)
{
    return m_pEditObject ? m_pEditObject->FindSMotionByName(name) : 0;
}

LPCSTR CActorTools::ExtractMotionName(LPCSTR full_name, LPCSTR prefix)
{
    LPCSTR _templ = "\\Slot ";
    if (0 == strstr(full_name, _templ))
        return full_name + xr_strlen(prefix) + 1;
    else
    {
        LPCSTR mot_nm = strstr(full_name, _templ);
        mot_nm += xr_strlen(_templ);
        while (isdigit(*mot_nm))
        {
            ++mot_nm;
        }
        return mot_nm + 1;
    }
    //    	return full_name + xr_strlen(prefix) + 1 + xr_strlen(_templ) + 1;
}

u16 CActorTools::ExtractMotionSlot(LPCSTR full_name, LPCSTR prefix)
{
    LPCSTR _templ = "\\Slot ";
    u16 slot = 0;
    LPCSTR slot_nm = strstr(full_name, _templ);
    if (0 != slot_nm)
    {
        string16 				tmp;
        slot_nm += xr_strlen(_templ);
        u32 ii = 0;
        while (isdigit(*slot_nm))
        {
            tmp[ii] = slot_nm[0];
            ++slot_nm;
            ++ii;
        }
        tmp[ii] = 0;
        slot = u16(atoi(tmp) - 1);
    }
    return slot;
}

xr_string CActorTools::BuildMotionPref(u16 slot, LPCSTR prefix)
{
    return "";
}


void CActorTools::OptimizeMotions()
{
    if (m_pEditObject) {
        m_pEditObject->OptimizeSMotions();
        Modified();
        UndoSave();
    }
}

void CActorTools::RealMakeThumbnail()
{
}

void CActorTools::RealGenerateLOD(bool hq)
{
}

bool CActorTools::BatchConvert(LPCSTR fn, int flags, shared_str script, float scale)
{
    bool bRes = false;
    CInifile* ini = CInifile::Create(fn); VERIFY(ini);
    if (ini->section_exist("ogf"))
    {
        bRes = true;
        CInifile::Sect& sect = ini->r_section("ogf");
        Msg("Start converting %d items...", sect.Data.size());
        for (auto it = sect.Data.begin(); it != sect.Data.end(); it++) 
        {
            string_path 		src_name;
            string_path 		tgt_name;

            std::string folder = fn;
            folder = folder.substr(0, folder.find_last_of("\\") + 1);
            std::string tgt = folder, src = folder;
            src += it->first.c_str();
            tgt += "meshes\\";
            tgt += it->second.c_str();

            xr_sprintf(src_name, "%s", src.c_str());
            xr_sprintf(tgt_name, "%s", tgt.c_str());

            strcpy(src_name, EFS.ChangeFileExt(src_name, ".object").c_str());
            strcpy(tgt_name, EFS.ChangeFileExt(tgt_name, ".ogf").c_str());
            if (FS.exist(src_name))
            {
                Msg(".Converting '%s' <-> '%s'", it->first.c_str(), it->second.c_str());
                CEditableObject* O = xr_new<CEditableObject>("convert");
                BOOL res = O->Load(src_name);
                O->a_vScale = scale;
                O->a_vAdjustMass = (flags & exfScaleCenterMass);

                if (O->BonePartCount() == 0 && O->IsSkeleton())
                {
                    ATools->ToDefaultBoneParts(O);
                    Msg("Can't find bone parts, reset to default.");
                }

                O->m_objectFlags.set(CEditableObject::eoProgressive, (flags & exfMakeProgressive));
                O->m_objectFlags.set(CEditableObject::eoStripify, (flags & exfMakeStripify));
                O->m_objectFlags.set(CEditableObject::eoOptimizeSurf, (flags & exfOptimizeSurfaces));
                O->m_objectFlags.set(CEditableObject::eoHQExportPlus, (flags & exfHQGeometryPlus));
                O->m_objectFlags.set(CEditableObject::eoExpBuildinMots, (flags & exfExportBuildInMots));
                O->m_objectFlags.set(CEditableObject::eoSoCSmooth, (flags & exfSoCSmooth));
                O->m_EditorScript = script;
                O->InitScript();

                if (ini->section_exist("skls_skl") && ini->line_exist("skls_skl", it->first.c_str()))
                {
                    string_path skls_name;
                    shared_str anim_to_load = ini->r_string("skls_skl", it->first.c_str());

                    for (int i = 0; i < _GetItemCount(anim_to_load.c_str()); i++)
                    {
                        string512 anim;
                        _GetItem(anim_to_load.c_str(), i, anim);

                        xr_sprintf(skls_name, "%s%s", folder.c_str(), anim);
                        strcpy(skls_name, EFS.ChangeFileExt(skls_name, ".skls").c_str());

                        if (!FS.exist(skls_name))
                            strcpy(skls_name, EFS.ChangeFileExt(skls_name, ".skl").c_str());

                        if (!FS.exist(skls_name))
                            Log("!Can't find anim:", skls_name);
                        else
                            O->AppendSMotion(skls_name);
                    }
                }

                if (O->SMotionCount() > 0)
                    O->m_SMotionRefs.clear();

                if (res) res = O->ExportOGF(tgt_name, 4);
                Log(res ? ".OK" : "!.FAILED");
                xr_delete(O);
            }
            else 
            {
                Log("!Invalid source file name:", it->first.c_str());
                bRes = false;
            }
        }
    }
    if (ini->section_exist("omf"))
    {
        bRes = true;
        CInifile::Sect& sect = ini->r_section("omf");
        Msg("Start converting %d items...", sect.Data.size());
        for (auto it = sect.Data.begin(); it != sect.Data.end(); ++it)
        {
            string_path 		src_name;
            string_path 		tgt_name;

            std::string folder = fn;
            folder = folder.substr(0, folder.find_last_of("\\") + 1);
            std::string tgt = folder, src = folder;
            src += it->first.c_str();
            tgt += "meshes\\";
            tgt += it->second.c_str();

            xr_sprintf(src_name, "%s", src.c_str());
            xr_sprintf(tgt_name, "%s", tgt.c_str());

            strcpy(src_name, EFS.ChangeFileExt(src_name, ".object").c_str());
            strcpy(tgt_name, EFS.ChangeFileExt(tgt_name, ".omf").c_str());
            if (FS.exist(src_name))
            {
                Msg(".Converting '%s' <-> '%s'", it->first.c_str(), it->second.c_str());
                CEditableObject* O = xr_new<CEditableObject>("convert");
                BOOL res = O->Load(src_name);
                O->a_vScale = scale;
                O->a_vAdjustMass = (flags & exfScaleCenterMass);

                if (O->BonePartCount() == 0 && O->IsSkeleton())
                {
                    ATools->ToDefaultBoneParts(O);
                    Msg("Can't find bone parts, reset to default.");
                }

                O->m_objectFlags.set(CEditableObject::eoExpBuildinMots, TRUE);
                O->m_EditorScript = script;
                O->InitScript();

                if (ini->section_exist("skls_skl") && ini->line_exist("skls_skl", it->first.c_str()))
                {
                    string_path skls_name;
                    shared_str anim_to_load = ini->r_string("skls_skl", it->first.c_str());

                    for (int i = 0; i < _GetItemCount(anim_to_load.c_str()); i++)
                    {
                        string512 anim;
                        _GetItem(anim_to_load.c_str(), i, anim);

                        xr_sprintf(skls_name, "%s%s", folder.c_str(), anim);
                        strcpy(skls_name, EFS.ChangeFileExt(skls_name, ".skls").c_str());

                        if (!FS.exist(skls_name))
                            strcpy(skls_name, EFS.ChangeFileExt(skls_name, ".skl").c_str());

                        if (!FS.exist(skls_name))
                            Log("!Can't find anim:", skls_name);
                        else
                            O->AppendSMotion(skls_name);
                    }
                }

                if (O->SMotionCount() > 0 && (flags & exfExportBuildInMots))
                    O->m_SMotionRefs.clear();

                if (res) res = O->ExportOMF(tgt_name);
                Log(res ? ".OK" : "!.FAILED");
                xr_delete(O);
            }
            else 
            {
                Log("!Invalid source file name:", it->first.c_str());
                bRes = false;
            }
        }
    }
    return bRes;
}

bool CActorTools::BatchConvertDialogOGF(xr_vector<BatchFiles> files, shared_str out, int flags, shared_str script, float scale)
{
    bool bRes = true;
    bool FileMode = (files.size() == 1 && files[0].source_folder == "null");

    for (int i = 0; i < files.size(); i++)
    {
        Msg("Start converting %d items...", files[i].files.size());
        for (int j = 0; j < files[i].files.size(); j++)
        {
            string_path 		src_name;
            string_path 		tgt_name;

            std::string tgt = out.c_str();

            if (FileMode)
            {
                const char* pFname;
                pFname = strrchr(files[i].files[j].c_str(), '\\');
                tgt += pFname;
            }
            else
            {
                std::string fname = files[i].files[j].c_str();
                fname.erase(0, files[i].source_folder.size());
                tgt += fname;
            }

            xr_sprintf(src_name, "%s", files[i].files[j].c_str());
            xr_sprintf(tgt_name, "%s", tgt.c_str());

            strcpy(tgt_name, EFS.ChangeFileExt(tgt_name, ".ogf").c_str());
            if (FS.exist(src_name))
            {
                CEditableObject* O = xr_new<CEditableObject>("convert");
                BOOL res = O->Load(src_name);
                O->a_vScale = scale;
                O->a_vAdjustMass = (flags & exfScaleCenterMass);

                if (O->BonePartCount() == 0 && O->IsSkeleton())
                {
                    ATools->ToDefaultBoneParts(O);
                    Msg("Can't find bone parts, reset to default.");
                }

                O->m_objectFlags.set(CEditableObject::eoProgressive, (flags & exfMakeProgressive));
                O->m_objectFlags.set(CEditableObject::eoStripify, (flags & exfMakeStripify));
                O->m_objectFlags.set(CEditableObject::eoOptimizeSurf, (flags & exfOptimizeSurfaces));
                O->m_objectFlags.set(CEditableObject::eoHQExportPlus, (flags & exfHQGeometryPlus));
                O->m_objectFlags.set(CEditableObject::eoExpBuildinMots, (flags & exfExportBuildInMots));
                O->m_objectFlags.set(CEditableObject::eoSoCSmooth, (flags & exfSoCSmooth));
                O->m_EditorScript = script;
                O->InitScript();

                shared_str skls_name = EFS.ChangeFileExt(src_name, ".skls").c_str();

                if (FS.exist(skls_name.c_str()))
                    O->AppendSMotion(skls_name.c_str());

                skls_name = EFS.ChangeFileExt(src_name, ".skl").c_str();
                if (FS.exist(skls_name.c_str()))
                    O->AppendSMotion(skls_name.c_str());

                if (O->SMotionCount() > 0 && (flags & exfExportBuildInMots))
                    O->m_SMotionRefs.clear();

                if (res) res = O->ExportOGF(tgt_name, 4);
                Log(res ? ".OK" : "!.FAILED");
                xr_delete(O);
            }
            else
            {
                bRes = false;
            }
        }
    }

    return bRes;
}

bool CActorTools::BatchConvertDialogOMF(xr_vector<BatchFiles> files, shared_str out, int flags, shared_str script, float scale)
{
    bool bRes = true;
    bool FileMode = (files.size() == 1 && files[0].source_folder == "null");

    for (int i = 0; i < files.size(); i++)
    {
        Msg("Start converting %d items...", files[i].files.size());
        for (int j = 0; j < files[i].files.size(); j++)
        {
            string_path 		src_name;
            string_path 		tgt_name;

            std::string tgt = out.c_str();

            if (FileMode)
            {
                const char* pFname;
                pFname = strrchr(files[i].files[j].c_str(), '\\');
                tgt += pFname;
            }
            else
            {
                std::string fname = files[i].files[j].c_str();
                fname.erase(0, files[i].source_folder.size());
                tgt += fname;
            }

            xr_sprintf(src_name, "%s", files[i].files[j].c_str());
            xr_sprintf(tgt_name, "%s", tgt.c_str());

            strcpy(tgt_name, EFS.ChangeFileExt(tgt_name, ".omf").c_str());
            if (FS.exist(src_name))
            {
                CEditableObject* O = xr_new<CEditableObject>("convert");
                BOOL res = O->Load(src_name);
                O->a_vScale = scale;
                O->a_vAdjustMass = (flags & exfScaleCenterMass);

                O->m_objectFlags.set(CEditableObject::eoExpBuildinMots, TRUE);
                O->m_EditorScript = script;
                O->InitScript();

                if (O->BonePartCount() == 0 && O->IsSkeleton())
                {
                    ATools->ToDefaultBoneParts(O);
                    Msg("Can't find bone parts, reset to default.");
                }

                shared_str skls_name = EFS.ChangeFileExt(src_name, ".skls").c_str();

                if (FS.exist(skls_name.c_str()))
                    O->AppendSMotion(skls_name.c_str());

                skls_name = EFS.ChangeFileExt(src_name, ".skl").c_str();
                if (FS.exist(skls_name.c_str()))
                    O->AppendSMotion(skls_name.c_str());

                if (O->SMotionCount() > 0 && (flags & exfExportBuildInMots))
                    O->m_SMotionRefs.clear();

                if (res) res = O->ExportOMF(tgt_name);
                Log(res ? ".OK" : "!.FAILED");
                xr_delete(O);
            }
            else
            {
                bRes = false;
            }
        }
    }

    return bRes;
}

void CActorTools::PhysicsSimulate()
{
}

void CActorTools::PhysicsStopSimulate()
{
    if (m_IsPhysics)
    {
        m_IsPhysics = false;
        m_RenderObject.DeletePhysicsShell();
        m_pEditObject->DeletePhysicsShell();
        DestroyPhysicsWorld();
    }
}

void CActorTools::CreatePhysicsWorld()
{
}

void CActorTools::DestroyPhysicsWorld()
{
}

bool CActorTools::GetSelectionPosition(Fmatrix& result)
{
    result = m_AVTransform;
    return true;
}

bool CActorTools::LoadBoneParts(LPCSTR full_name)
{
    PrepareBoneParts(m_pEditObject);

    if (FS.exist(full_name)) 
    {
        for (int k = 0; k < 4; k++) { m_List[k].clear(); m_Name[k][0] = 0; }
        CInifile ini(full_name, TRUE, TRUE, FALSE);
        string64		buff;
        for (int i = 0; i < 4; ++i)
        {
            sprintf(buff, "part_%d", i);
            sprintf(m_Name[i], "%s", ini.r_string(buff, "partition_name"));
            CInifile::Sect& S = ini.r_section(buff);
            CInifile::SectCIt it = S.Data.begin();
            CInifile::SectCIt e = S.Data.end();
            for (; it != e; ++it)
            {
                if (0 != stricmp(it->first.c_str(), "partition_name"))
                {
                    m_List[i].push_back(it->first);
                }
            }
        }
    }
    return UpdateBoneParts(m_pEditObject);
}

bool CActorTools::SaveBoneParts(LPCSTR full_name)
{
    PrepareBoneParts(m_pEditObject);

    CInifile ini(full_name, FALSE, FALSE, TRUE);
    string64		buff;
    for (int i = 0; i < 4; ++i)
    {
        sprintf(buff, "part_%d", i);
        ini.w_string(buff, "partition_name", m_Name[i]);
        for (auto node : m_List[i])
        {
            ini.w_string(buff, node.name.c_str(), NULL);
        }
    }
    return true;
}

bool CActorTools::ToDefaultBoneParts(CEditableObject* object)
{
    PrepareBoneParts(object);

    for (int k = 0; k < 4; k++) { m_List[k].clear(); m_Name[k][0] = 0; }
    xr_strcpy(m_Name[0], "default");
    for (BoneIt it = object->FirstBone(); it != object->LastBone(); it++)
    {
        m_List[0].push_back((*it)->Name());
    }
    return UpdateBoneParts(object);
}

bool CActorTools::UpdateBoneParts(CEditableObject* object)
{
    for (int k = 0; k < 4; k++)
    {
        if (m_List[k].size()&&!xr_strlen(m_Name[k])) 
        {
            ELog.DlgMsg(mtError, "Verify parts name.");
            return false;
        }
        for (int i = 0; i < 4; i++)
        {
            if (i == k)continue;
            if (!m_List[k].size()) continue;
            string_path Name[2];
            xr_strcpy(Name[0], m_Name[k]);
            xr_strcpy(Name[1], m_Name[i]);
            _strupr_s(Name[0]); _strupr_s(Name[1]);
            if (xr_strcmp(Name[0], Name[1])==0)
            {
                ELog.DlgMsg(mtError, "Unique name required.");
                return false;
            }
        }
    }

    // verify
    U8Vec b_use(object->BoneCount(), 0);
    for (int k = 0; k < 4; k++)
    {
        if (m_List[k].size())
        {
            for (auto node : m_List[k])
            {
                b_use[object->FindBoneByNameIdx(node.name.c_str())]++;
            }

        }

    }
    for (U8It u_it = b_use.begin(); u_it != b_use.end(); u_it++)
    {
        if (*u_it != 1)
        {
            ELog.DlgMsg(mtError, "Invalid bone part found (missing or duplicate bones).");
            return false;
        }
    }
    // save    
    object->m_BoneParts.clear();
    for (int k = 0; k < 4; k++) 
    {
        if (m_List[k].size())
        {
            object->m_BoneParts.push_back(SBonePart());
            SBonePart& BP = object->m_BoneParts.back();
            BP.alias = m_Name[k];
            for (auto node : m_List[k])
            {
                BP.bones.push_back(node.name);
            }

        }
    }

    return true;
}

bool CActorTools::PrepareBoneParts(CEditableObject* object)
{
    for (int k = 0; k < 4; k++) { m_List[k].clear(); m_Name[k][0] = 0; }
    for (BPIt it = object->m_BoneParts.begin(); it != object->m_BoneParts.end(); it++) 
    {
        xr_strcpy(m_Name[it - object->m_BoneParts.begin()], it->alias.c_str());
        for (RStringVecIt w_it = it->bones.begin(); w_it != it->bones.end(); w_it++)
            m_List[it - object->m_BoneParts.begin()].push_back(*w_it);
    }
    return true;
}

bool CActorTools::BonePartsExist()
{
    return m_pEditObject->m_BoneParts.size() > 0;
}

void PreviewModel::OnCreate()
{
    
}

void PreviewModel::OnDestroy()
{
}

void PreviewModel::Clear()
{
}

void PreviewModel::SelectObject()
{
}

void PreviewModel::SetPreferences()
{
}

void PreviewModel::Render()
{
}

void PreviewModel::Update()
{
}



void CActorTools::PrepareLighting()
{
    // add directional light
    Flight L;
    ZeroMemory(&L, sizeof(Flight));
    L.type = D3DLIGHT_DIRECTIONAL;

    L.diffuse.set(1, 1, 1, 1);
    L.direction.set(1, -1, 1); L.direction.normalize();
    EDevice->SetLight(0, L);
    EDevice->LightEnable(0, true);

    L.diffuse.set(0.2, 0.2, 0.2, 1);
    L.direction.set(-1, -1, -1); L.direction.normalize();
    EDevice->SetLight(1, L);
    EDevice->LightEnable(1, true);

    L.diffuse.set(0.2, 0.2, 0.2, 1);
    L.direction.set(1, -1, -1); L.direction.normalize();
    EDevice->SetLight(2, L);
    EDevice->LightEnable(2, true);

    L.diffuse.set(0.2, 0.2, 0.2, 1);
    L.direction.set(-1, -1, 1); L.direction.normalize();
    EDevice->SetLight(3, L);
    EDevice->LightEnable(3, true);

    L.diffuse.set(1.0, 0.4, 0.3, 1);
    L.direction.set(0, 1, 0); L.direction.normalize();
    EDevice->SetLight(4, L);
    EDevice->LightEnable(4, true);

    L.diffuse.set(0.3, 1.0, 0.4, 1);
    L.direction.set(-1, -1, -1); L.direction.normalize();
    EDevice->SetLight(5, L);
    EDevice->LightEnable(5, true);
}
