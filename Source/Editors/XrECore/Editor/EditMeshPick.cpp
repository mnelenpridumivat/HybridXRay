//----------------------------------------------------
// file: StaticMesh.cpp
//----------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "EditMesh.h"
#include "EditObject.h"
//#include "cl_collector.h"
#include "ui_main.h"
#include "pick_defs.h"

/*
void CEditableMesh::CHullPickFaces(PlaneVec& pl, Fmatrix& parent, U32Vec& fl){
	u32 i=0;
	Fvector p;
    vector<bool> inside(m_Points.size(),true);
    for(FvectorIt v_it=m_Points.begin();v_it!=m_Points.end();v_it++){
        parent.transform_tiny(p,*v_it);
        for(PlaneIt p_it=pl.begin(); p_it!=pl.end(); p_it++)
        	if (p_it->classify(p)>EPS_L) { inside[v_it-m_Points.begin()]=false; break; }
    }
    for(FaceIt f_it=m_Faces.begin();f_it!=m_Faces.end();f_it++,i++)
    	if (inside[f_it->pv[0].pindex]&&inside[f_it->pv[1].pindex]&&inside[f_it->pv[2].pindex]) fl.push_back(i);
}
*/
//----------------------------------------------------

static IntVec		sml_processed;
static Fvector		sml_normal;
static float		m_fSoftAngle;
//----------------------------------------------------

//----------------------------------------------------
// номер face должен соответствовать списку
//----------------------------------------------------
void CEditableMesh::GenerateCFModel()
{
	UnloadCForm		();
}

void CEditableMesh::RayQuery(SPickQuery& pinf)
{
}

void CEditableMesh::RayQuery(const Fmatrix& parent, const Fmatrix& inv_parent, SPickQuery& pinf)
{
}

void CEditableMesh::BoxQuery(const Fmatrix& parent, const Fmatrix& inv_parent, SPickQuery& pinf)
{
}

static const float _sqrt_flt_max = _sqrt(flt_max*0.5f);

bool CEditableMesh::RayPick(float& distance, const Fvector& start, const Fvector& direction, const Fmatrix& inv_parent, SRayPickInfo* pinf)
{
	if (!m_Flags.is(flVisible)) return false;
	return false;
}
//----------------------------------------------------
#if 1
bool CEditableMesh::CHullPickMesh(PlaneVec& pl, const Fmatrix& parent)
{
	Fvector p;
    boolVec inside;
    inside.assign(m_VertCount, true);
    for(u32 v_id=0;v_id<m_VertCount;v_id++){
        parent.transform_tiny(p,m_Vertices[v_id]);
        for(PlaneIt p_it=pl.begin(); p_it!=pl.end(); p_it++)
        	if (p_it->classify(p)>EPS_L) { inside[v_id]=false; break; }
    }
    for(u32 f_id=0;f_id<m_FaceCount;f_id++)
    	if (inside[m_Faces[f_id].pv[0].pindex]&&inside[m_Faces[f_id].pv[1].pindex]&&inside[m_Faces[f_id].pv[2].pindex]) return true;
    return false;
}
//----------------------------------------------------

void CEditableMesh::RecurseTri(int id)
{
	// Check if triangle already processed
	if (std::find(sml_processed.begin(),sml_processed.end(),id)!=sml_processed.end())
		return;

	sml_processed.push_back(id);

    // recurse
    for (int k=0; k<3; k++){
	    IntVec& PL = (*m_Adjs)[m_Faces[id].pv[k].pindex];
		for (IntIt pl_it=PL.begin(); pl_it!=PL.end(); pl_it++){
           	Fvector &test_normal = m_FaceNormals[*pl_it];
           	float cosa = test_normal.dotproduct(sml_normal);
           	if (cosa<m_fSoftAngle) continue;
     		RecurseTri(*pl_it);
        }
    }
}
//----------------------------------------------------

void CEditableMesh::GetTiesFaces(int start_id, U32Vec& fl, float fSoftAngle, bool bRecursive)
{
    R_ASSERT(start_id<int(m_FaceCount));
    m_fSoftAngle=cosf(deg2rad(fSoftAngle));
    GenerateFNormals	();
	GenerateAdjacency	();
    VERIFY				(m_FaceNormals);
    if (bRecursive){
    	sml_processed.clear();
        sml_normal.set(m_FaceNormals[start_id]);
    	RecurseTri(start_id);
        fl.insert(fl.begin(),sml_processed.begin(),sml_processed.end());
    }else{
    	for (int k=0; k<3; k++)
        	fl.insert(fl.end(),(*m_Adjs)[m_Faces[start_id].pv[k].pindex].begin(),(*m_Adjs)[m_Faces[start_id].pv[k].pindex].end());
        std::sort(fl.begin(),fl.end());
        fl.erase(std::unique(fl.begin(),fl.end()),fl.end());
    }
    UnloadFNormals		();
	UnloadAdjacency		();
}
//----------------------------------------------------

//----------------------------------------------------
#endif //



