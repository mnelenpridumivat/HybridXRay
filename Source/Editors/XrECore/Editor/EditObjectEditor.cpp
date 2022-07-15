#include "stdafx.h"
#pragma hdrstop

#include "EditObject.h"
#include "EditMesh.h"
#include "..\xrEngine\motion.h"
#include "..\xrEngine\bone.h"
#include "ExportSkeleton.h"
#include "ExportObjectOGF.h"
#include "d3dutils.h"
#include "ui_main.h"
#include "render.h"
#include "ResourceManager.h"

const float tex_w	= LOD_SAMPLE_COUNT*LOD_IMAGE_SIZE;
const float tex_h	= 1*LOD_IMAGE_SIZE;
const float half_p_x= 0.5f*(1.f/tex_w);
const float half_p_y= 0.5f*(1.f/tex_h);
const float offs_x 	= 1.f/tex_w;
const float offs_y 	= 1.f/tex_h;

static Fvector LOD_pos[4]={
	{-1.0f+offs_x, 1.0f-offs_y, 0.0f},
	{ 1.0f-offs_x, 1.0f-offs_y, 0.0f},
	{ 1.0f-offs_x,-1.0f+offs_y, 0.0f},
	{-1.0f+offs_x,-1.0f+offs_y, 0.0f}
};
static FVF::LIT LOD[4]={
	{{-1.0f, 1.0f, 0.0f},  0xFFFFFFFF, {0.0f,0.0f}}, // F 0
	{{ 1.0f, 1.0f, 0.0f},  0xFFFFFFFF, {0.0f,0.0f}}, // F 1
	{{ 1.0f,-1.0f, 0.0f},  0xFFFFFFFF, {0.0f,0.0f}}, // F 2
	{{-1.0f,-1.0f, 0.0f},  0xFFFFFFFF, {0.0f,0.0f}}, // F 3
};

bool CEditableObject::Reload()
{
	ClearGeometry();
    return Load(m_LoadName.c_str());
}

bool CEditableObject::RayPick(float& dist, const Fvector& S, const Fvector& D, const Fmatrix& inv_parent, SRayPickInfo* pinf)
{
	bool picked = false;
    for(EditMeshIt m = m_Meshes.begin();m!=m_Meshes.end();m++)
        if( (*m)->RayPick( dist, S, D, inv_parent, pinf ) )
            picked = true;
	return picked;
}

void CEditableObject::RayQuery(SPickQuery& pinf)
{
    for(EditMeshIt m = m_Meshes.begin();m!=m_Meshes.end();m++)
        (*m)->RayQuery(pinf);
}

void CEditableObject::RayQuery(const Fmatrix& parent, const Fmatrix& inv_parent, SPickQuery& pinf)
{
    for(EditMeshIt m = m_Meshes.begin();m!=m_Meshes.end();m++)
        (*m)->RayQuery(parent, inv_parent, pinf);
}

void CEditableObject::BoxQuery(const Fmatrix& parent, const Fmatrix& inv_parent, SPickQuery& pinf)
{
    for(EditMeshIt m = m_Meshes.begin();m!=m_Meshes.end();m++)
        (*m)->BoxQuery(parent, inv_parent, pinf);
}

#if 1
bool CEditableObject::FrustumPick(const CFrustum& frustum, const Fmatrix& parent){
	return false;
}

#endif

extern float 	ssaLIMIT;
extern float	g_fSCREEN;
static const float ssaLim = 64.f*64.f/(640*480);
void CEditableObject::Render(const Fmatrix& parent, int priority, bool strictB2F, SurfaceVec* surfaces){
}

void CEditableObject::RenderSingle(const Fmatrix& parent)
{
	for (int i=0; i<4; i++){
		Render(parent, i, false);
		Render(parent, i, true);
    }
}

void CEditableObject::RenderAnimation(const Fmatrix&){
}

void CEditableObject::RenderEdge(const Fmatrix& parent, CEditableMesh* mesh, CSurface* surf, u32 color)
{
    if (!(m_LoadState.is(LS_RBUFFERS))) DefferedLoadRP();

    EDevice->SetShader(EDevice->m_WireShader);
    if(mesh) mesh->RenderEdge(parent, surf, color);
    else for(EditMeshIt _M = m_Meshes.begin();_M!=m_Meshes.end();_M++)
            (*_M)->RenderEdge(parent, surf, color);
}

void CEditableObject::RenderSelection(const Fmatrix& parent, CEditableMesh* mesh, CSurface* surf, u32 color)
{
    if (!(m_LoadState.is(LS_RBUFFERS))) DefferedLoadRP();

    RCache.set_xform_world(parent);
    EDevice->SetShader(EDevice->m_SelectionShader);
    EDevice->RenderNearer(0.0005);
    if(mesh) mesh->RenderSelection(parent, surf, color);
    else for(EditMeshIt _M = m_Meshes.begin();_M!=m_Meshes.end();_M++)
         	(*_M)->RenderSelection(parent, surf, color);
    EDevice->ResetNearer();
}

IC static void CalculateLODTC(int frame, int w_cnt, int h_cnt, Fvector2& lt, Fvector2& rb)
{
	Fvector2	ts;
    ts.set		(1.f/(float)w_cnt,1.f/(float)h_cnt);
    lt.x        = (frame%w_cnt+0)*ts.x+half_p_x;
    lt.y        = (frame/w_cnt+0)*ts.y+half_p_y;
    rb.x        = (frame%w_cnt+1)*ts.x-half_p_x;
    rb.y        = (frame/w_cnt+1)*ts.y-half_p_y;
}

void CEditableObject::GetLODFrame(int frame, Fvector p[4], Fvector2 t[4], const Fmatrix* parent)
{
	R_ASSERT(m_objectFlags.is(eoUsingLOD));
    Fvector P,S;
    m_BBox.get_CD	(P,S);
    float r 		= _max(S.x,S.z);//sqrtf(S.x*S.x+S.z*S.z);
    Fmatrix T,matrix,rot;
    T.scale			(r,S.y,r);
    T.translate_over(P);
    if (parent) 
    	T.mulA_43(*parent);

    float angle 	= frame*(PI_MUL_2/float(LOD_SAMPLE_COUNT));
    rot.rotateY(-angle);
    matrix.mul(T,rot);
    Fvector2 lt, rb;
    CalculateLODTC(frame,LOD_SAMPLE_COUNT,1,lt,rb);
    t[0].set(lt);
    t[1].set(rb.x,lt.y);
    t[2].set(rb);
    t[3].set(lt.x,rb.y);
    matrix.transform_tiny(p[0],LOD_pos[0]);
    matrix.transform_tiny(p[1],LOD_pos[1]);
    matrix.transform_tiny(p[2],LOD_pos[2]);
    matrix.transform_tiny(p[3],LOD_pos[3]);
}

void CEditableObject::RenderLOD(const Fmatrix& parent)
{
}

xr_string CEditableObject::GetLODTextureName()
{
    return "";
}

void CEditableObject::OnDeviceCreate()
{
}

void CEditableObject::OnDeviceDestroy()
{
	DefferedUnloadRP();
}

void CEditableObject::DefferedLoadRP()
{
	if (m_LoadState.is(LS_RBUFFERS)) return;

    // skeleton
	if (IsSkeleton())
		vs_SkeletonGeom.create(FVF_SV,RCache.Vertex.Buffer(),RCache.Index.Buffer());

//*/
	// ������� LOD shader
	xr_string l_name = GetLODTextureName();
    xr_string fname = xr_string(l_name)+xr_string(".dds");
    m_LODShader.destroy();
//    if (FS.exist(_game_textures_,fname.c_str()))
    if (m_objectFlags.is(eoUsingLOD))
    	m_LODShader.create(GetLODShaderName(),l_name.c_str());
    m_LoadState.set(LS_RBUFFERS,TRUE);
}
void CEditableObject::DefferedUnloadRP()
{
	if (!(m_LoadState.is(LS_RBUFFERS))) return;
    // skeleton
	vs_SkeletonGeom.destroy();
    // ������� ������
	for (EditMeshIt _M=m_Meshes.begin(); _M!=m_Meshes.end(); _M++)
    	if (*_M) (*_M)->GenerateRenderBuffers();
	// ������� shaders
    for(SurfaceIt s_it=m_Surfaces.begin(); s_it!=m_Surfaces.end(); s_it++)
        (*s_it)->OnDeviceDestroy();
    // LOD
    m_LODShader.destroy();
    m_LoadState.set(LS_RBUFFERS,FALSE);
}
void CEditableObject::EvictObject()
{
	EditMeshIt m 				= m_Meshes.begin();
	for(;m!=m_Meshes.end();m++){
    	(*m)->UnloadCForm		();
    	(*m)->UnloadVNormals	(true);
        (*m)->UnloadSVertices	(true);
        (*m)->UnloadFNormals	(true);
    }
    DefferedUnloadRP			();
}

//bool CEditableObject::PrepareOGF(IWriter& F, u8 infl, bool gen_tb, CEditableMesh* mesh)
//{
//	return IsSkeleton()?PrepareSkeletonOGF(F,infl):PrepareRigidOGF(F,gen_tb,mesh);
//}
//
//bool CEditableObject::PrepareRigidOGF(IWriter& F, bool gen_tb, CEditableMesh* mesh)
//{
//    CExportObjectOGF E(this);
//    return E.Export(F,gen_tb,mesh);
//}
//
//bool CEditableObject::PrepareSVGeometry(IWriter& F, u8 infl)
//{
//    CExportSkeleton E(this);
//    return E.ExportGeometry(F, infl);
//}
//
//bool CEditableObject::PrepareSVKeys(IWriter& F)
//{
//    CExportSkeleton E(this);
//    return E.ExportMotionKeys(F);
//}
//
//bool CEditableObject::PrepareSVDefs(IWriter& F)
//{
//    CExportSkeleton E(this);
//    return E.ExportMotionDefs(F);
//}
//
//bool CEditableObject::PrepareSkeletonOGF(IWriter& F, u8 infl)
//{
//    CExportSkeleton E(this);
//    return E.Export(F,infl);
//}
//
//bool CEditableObject::PrepareOMF(IWriter& F)
//{
//    CExportSkeleton E(this);
//    return E.ExportMotions(F);
//}
//---------------------------------------------------------------------------

void  CEditableObject::OnChangeTransform(PropValue*)
{
	UI->RedrawScene();
}
//---------------------------------------------------------------------------

//. #include "../../xrEngine/blenders/Blender.h"
IC BOOL BE      (BOOL A, BOOL B)
{
    bool a = !!A;
    bool b = !!B;
    return a==b;
}
bool CEditableObject::CheckShaderCompatible()
{
	bool bRes 			= true;
    return bRes;
}
#if 1
void CEditableObject::AddBone(CBone* parent_bone, shared_str name)
{
	CBone* B 			= xr_new<CBone>();

 	B->SetName			(name.c_str());
    B->SetWMap			("");
	B->SetRestParams	(0.01f, Fvector().set(0,0,0), Fvector().set(0,0,0) );

    B->ResetData		();
	B->Reset			();

	if(parent_bone)
    {
   		B->SetParentName(parent_bone->Name().c_str());
    }
    else
    {
   		B->SetParentName				("");
        B->parent = 0;
    }

    if (m_BoneParts.size() == 0)
    {
        m_BoneParts.push_back(SBonePart());
        SBonePart& BP = m_BoneParts.back();
        BP.alias = "default";
    }

    m_BoneParts[0].bones.push_back(B->Name());
    m_Bones.push_back	(B);
    m_objectFlags.set(eoDynamic, TRUE);
	PrepareBones		();

    for (EditMeshIt _M=m_Meshes.begin(); _M!=m_Meshes.end(); _M++)
	{
    	(*_M)->UnloadSVertices();
    }
}

CBone* 	bone_to_delete = NULL;
u32 	bone_to_delete_frame = 0;

void CEditableObject::DeleteBone(CBone* bone)
{
	CBone* PB = bone->Parent();

    for (BoneIt b_it=m_Bones.begin(); b_it!=m_Bones.end(); ++b_it)
    {
    	CBone* B = *b_it;
        if(B->Parent()==bone)
        	B->SetParentName(PB?PB->Name().c_str():"");
    }
    BoneIt bit 		= std::find(m_Bones.begin(), m_Bones.end(), bone);
    if(bit==m_Bones.end())
    	Msg("! bone not found -((");
    else
    	m_Bones.erase	(bit);

	BPIt bpit 			= BonePart(bone);
    RStringVec::iterator iit 	= (*bpit).bones.begin();
    RStringVec::iterator iit_e 	= (*bpit).bones.end();
    for( ;iit!=iit_e; ++iit)
    {
        if(*iit==bone->Name())
        {
            (*bpit).bones.erase(iit);
            break;
        }
    }


    bone_to_delete = bone;
    bone_to_delete_frame = 0;
	PrepareBones	();

    for (EditMeshIt _M=m_Meshes.begin(); _M!=m_Meshes.end(); _M++)
	{
    	(*_M)->UnloadSVertices();
    }
}
#endif
//---------------------------------------------------------------------------

BPIt CEditableObject::BonePart(CBone* B)
{
	BPIt it 		= FirstBonePart();
	BPIt it_e 		= LastBonePart();
    for( ;it!=it_e; ++it)
    {
    	RStringVec::iterator iit 	= (*it).bones.begin();
    	RStringVec::iterator iit_e 	= (*it).bones.end();
    	for( ;iit!=iit_e; ++iit)
        {
        	if(*iit==B->Name())
            	return it;
        }
    }
    return it_e;
}

void CEditableObject::RenameBone(CBone* bone, LPCSTR new_name)
{
	BPIt bpit 			= BonePart(bone);
    RStringVec::iterator iit 	= (*bpit).bones.begin();
    RStringVec::iterator iit_e 	= (*bpit).bones.end();

    for( ;iit!=iit_e; ++iit)
    {
        if(*iit==bone->Name())
        {
            *iit = new_name;
            break;
        }
    }

    for (BoneIt b_it=m_Bones.begin(); b_it!=m_Bones.end(); ++b_it)
    {
    	CBone* B = *b_it;
        if(B->ParentName()==bone->Name())
        	B->SetParentName(new_name);
    }

    for (SMotionIt s_it=m_SMotions.begin(); s_it!=m_SMotions.end(); ++s_it) 
    {
    	CSMotion* M = *s_it;
        for(BoneMotionIt bm_it=M->BoneMotions().begin(); bm_it!=M->BoneMotions().end(); ++bm_it)
        {
            if(bm_it->name == bone->Name())
                bm_it->name = new_name;
        }
        
    }

	//bone->SetName(new_name);
    
    Modified();
}

