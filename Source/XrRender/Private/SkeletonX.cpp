// SkeletonX.cpp: implementation of the CSkeletonX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#pragma warning(disable:4995)
#include "directx\d3dx9.h"
#pragma warning(default:4995)

#ifndef REDITOR
	#include	"../../xrEngine/Render.h"
#else
#include "../../xrAPI/xrAPI.h"
#endif

#include "SkeletonX.h"
#include "SkeletonCustom.h"
#include "../xrEngine/fmesh.h"

shared_str	s_bones_array_const;

//////////////////////////////////////////////////////////////////////
// Body Part
//////////////////////////////////////////////////////////////////////
void CSkeletonX::AfterLoad	(CKinematics* parent, u16 child_idx)
{
	SetParent				(parent);
    ChildIDX				= child_idx;
}
void CSkeletonX::_Copy		(CSkeletonX *B)
{
	Parent					= NULL;
	ChildIDX				= B->ChildIDX;
	Vertices1W				= B->Vertices1W;
	Vertices2W				= B->Vertices2W;
	Vertices3W				= B->Vertices3W;
	Vertices4W				= B->Vertices4W;
	BonesUsed				= B->BonesUsed;

	// caution - overlapped (union)
	cache_DiscardID			= B->cache_DiscardID;
	cache_vCount			= B->cache_vCount;
	cache_vOffset			= B->cache_vOffset;
	RenderMode				= B->RenderMode;
	RMS_boneid				= B->RMS_boneid;
	RMS_bonecount			= B->RMS_bonecount;

#if defined(USE_DX10) || defined(USE_DX11)
	m_Indices				= B->m_Indices;
#endif	//	USE_DX10
}
//////////////////////////////////////////////////////////////////////
void CSkeletonX::_Render	(ref_geom& hGeom, u32 vCount, u32 iOffset, u32 pCount)
{

}
void CSkeletonX::_Render_soft	(ref_geom& hGeom, u32 vCount, u32 iOffset, u32 pCount)
{
}

void CSkeletonX::_Load	(const char* N, IReader *data, u32& dwVertCount) 
{	
}

BOOL CSkeletonX::has_visible_bones()
{
	if	(RM_SINGLE==RenderMode)	
	{
		return Parent->LL_GetBoneVisible((u16)RMS_boneid);
	}

	for (u32 it=0; it<BonesUsed.size(); it++)
		if (Parent->LL_GetBoneVisible(BonesUsed[it]))	
		{
			return	TRUE;
		}
	return	FALSE;
}


void 	get_pos_bones(const vertBoned1W &v, Fvector& p, CKinematics* Parent )
{
	const Fmatrix& xform	= Parent->LL_GetBoneInstance((u16)v.matrix).mRenderTransform; 
	xform.transform_tiny	( p, v.P );
}

void 	get_pos_bones(const vertBoned2W &vert, Fvector& p, CKinematics* Parent )
{
	Fvector		P0,P1;
	
	Fmatrix& xform0			= Parent->LL_GetBoneInstance( vert.matrix0 ).mRenderTransform; 
	Fmatrix& xform1			= Parent->LL_GetBoneInstance( vert.matrix1 ).mRenderTransform; 
	xform0.transform_tiny	( P0, vert.P );
	xform1.transform_tiny	( P1, vert.P );
	p.lerp					( P0, P1, vert.w );
}

void 	get_pos_bones(const vertBoned3W &vert, Fvector& p, CKinematics* Parent )
{
		Fmatrix& M0		= Parent->LL_GetBoneInstance( vert.m[0] ).mRenderTransform;
        Fmatrix& M1		= Parent->LL_GetBoneInstance( vert.m[1] ).mRenderTransform;
        Fmatrix& M2		= Parent->LL_GetBoneInstance( vert.m[2] ).mRenderTransform;

		Fvector	P0,P1,P2;
		M0.transform_tiny(P0, vert.P); P0.mul(vert.w[0]);
        M1.transform_tiny(P1, vert.P); P1.mul(vert.w[1]);
        M2.transform_tiny(P2, vert.P); P2.mul(1.0f-vert.w[0]-vert.w[1]);

		p			= P0;
		p.add		(P1);
		p.add		(P2);
}
void 	get_pos_bones(const vertBoned4W &vert, Fvector& p, CKinematics* Parent )
{
		Fmatrix& M0		= Parent->LL_GetBoneInstance( vert.m[0] ).mRenderTransform;
        Fmatrix& M1		= Parent->LL_GetBoneInstance( vert.m[1] ).mRenderTransform;
        Fmatrix& M2		= Parent->LL_GetBoneInstance( vert.m[2] ).mRenderTransform;
		Fmatrix& M3		= Parent->LL_GetBoneInstance( vert.m[3] ).mRenderTransform;

		Fvector	P0,P1,P2,P3;
		M0.transform_tiny(P0, vert.P); P0.mul(vert.w[0]);
        M1.transform_tiny(P1, vert.P); P1.mul(vert.w[1]);
		M2.transform_tiny(P2, vert.P); P2.mul(vert.w[2]);
        M3.transform_tiny(P3,vert.P); P3.mul(1.0f-vert.w[0]-vert.w[1]-vert.w[2]);
		
		p			= P0;
		p.add		(P1);
		p.add		(P2);
		p.add		(P3);
}

//-----------------------------------------------------------------------------------------------------
// Wallmarks
//-----------------------------------------------------------------------------------------------------
#include "cl_intersect.h"
BOOL	CSkeletonX::_PickBoneSoft1W	(IKinematics::pick_result &r, float dist, const Fvector& S, const Fvector& D, u16* indices, CBoneData::FacesVec& faces)
{
		return pick_bone<vertBoned1W>( Vertices1W, Parent, r, dist, S, D, indices, faces);
}

BOOL CSkeletonX::_PickBoneSoft2W	(IKinematics::pick_result &r, float dist, const Fvector& S, const Fvector& D, u16* indices, CBoneData::FacesVec& faces)
{
		return pick_bone<vertBoned2W>( Vertices2W, Parent,r, dist, S, D, indices, faces);
}

BOOL CSkeletonX::_PickBoneSoft3W	(IKinematics::pick_result &r, float dist, const Fvector& S, const Fvector& D, u16* indices, CBoneData::FacesVec& faces)
{
		return pick_bone<vertBoned3W>( Vertices3W, Parent, r, dist, S, D, indices, faces);
}

BOOL CSkeletonX::_PickBoneSoft4W	(IKinematics::pick_result &r, float dist, const Fvector& S, const Fvector& D, u16* indices, CBoneData::FacesVec& faces)
{
		return pick_bone<vertBoned4W>( Vertices4W, Parent, r, dist, S, D, indices, faces);
}
/*
BOOL	CSkeletonX::_PickBoneSoft1W	(Fvector& normal, float& dist, const Fvector& S, const Fvector& D, u16* indices, CBoneData::FacesVec& faces)
{
	VERIFY				(*Vertices1W);
	bool intersect		= FALSE;
	for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++){
		Fvector			p[3];
		u32 idx			= (*it)*3;
		for (u32 k=0; k<3; k++){
			vertBoned1W& vert		= Vertices1W[indices[idx+k]];
			get_pos_bones(vert, p[k], Parent);
		}
		float u,v,range	= flt_max;
		if (CDB::TestRayTri(S,D,p,u,v,range,true)&&(range<dist)){
			normal.mknormal(p[0],p[1],p[2]);
			dist		= range;
			intersect	= TRUE;
		}
	}
	return intersect;
}

BOOL CSkeletonX::_PickBoneSoft2W	(Fvector& normal, float& dist, const Fvector& S, const Fvector& D, u16* indices, CBoneData::FacesVec& faces)
{
	VERIFY				(*Vertices2W);
	bool intersect		= FALSE;
	for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++){
		Fvector			p[3];
		u32 idx			= (*it)*3;
		for (u32 k=0; k<3; k++){
			vertBoned2W& vert		= Vertices2W[indices[idx+k]];
			get_pos_bones(vert, p[k], Parent);
		}
		float u,v,range	= flt_max;
		if (CDB::TestRayTri(S,D,p,u,v,range,true)&&(range<dist)){
			normal.mknormal(p[0],p[1],p[2]);
			dist		= range;
			intersect	= TRUE;
		}
	}
	return intersect;
}
*/
// Fill Vertices
void CSkeletonX::_FillVerticesSoft1W(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, u16* indices, CBoneData::FacesVec& faces)
{
	VERIFY				(*Vertices1W);
	for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++){
		Fvector			p[3];
		u32 idx			= (*it)*3;
		CSkeletonWallmark::WMFace F;
		for (u32 k=0; k<3; k++){
			vertBoned1W& vert		= Vertices1W[indices[idx+k]];
			F.bone_id[k][0]			= (u16)vert.matrix;
			F.bone_id[k][1]			= F.bone_id[k][0];
			F.weight[k]				= 0.f;
			const Fmatrix& xform	= Parent->LL_GetBoneInstance(F.bone_id[k][0]).mRenderTransform; 
			F.vert[k].set			(vert.P);
			xform.transform_tiny	(p[k],F.vert[k]);
		}
		Fvector test_normal;
		test_normal.mknormal	(p[0],p[1],p[2]);
		float cosa				= test_normal.dotproduct(normal);
		if (cosa<EPS)			continue;
		if (CDB::TestSphereTri(wm.ContactPoint(),size,p))
		{
			Fvector				UV;
			for (u32 k=0; k<3; k++){
				Fvector2& uv	= F.uv[k];
				view.transform_tiny(UV,p[k]);
				uv.x			= (1+UV.x)*.5f;
				uv.y			= (1-UV.y)*.5f;
			}
			wm.m_Faces.push_back(F);
		}
	}
}
void CSkeletonX::_FillVerticesSoft2W(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, u16* indices, CBoneData::FacesVec& faces)
{
	VERIFY				(*Vertices2W);
	for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++){
		Fvector			p[3];
		u32 idx			= (*it)*3;
		CSkeletonWallmark::WMFace F;
		for (u32 k=0; k<3; k++){
			Fvector		P0,P1;
			vertBoned2W& vert		= Vertices2W[indices[idx+k]];
			F.bone_id[k][0]			= vert.matrix0;
			F.bone_id[k][1]			= vert.matrix1;
			F.weight[k]				= vert.w;
			Fmatrix& xform0			= Parent->LL_GetBoneInstance(F.bone_id[k][0]).mRenderTransform; 
			Fmatrix& xform1			= Parent->LL_GetBoneInstance(F.bone_id[k][1]).mRenderTransform; 
			F.vert[k].set			(vert.P);		
			xform0.transform_tiny	(P0,F.vert[k]);
			xform1.transform_tiny	(P1,F.vert[k]);
			p[k].lerp				(P0,P1,F.weight[k]);
		}
		Fvector test_normal;
		test_normal.mknormal	(p[0],p[1],p[2]);
		float cosa				= test_normal.dotproduct(normal);
		if (cosa<EPS)			continue;
		if (CDB::TestSphereTri(wm.ContactPoint(),size,p)){
			Fvector				UV;
			for (u32 k=0; k<3; k++){
				Fvector2& uv	= F.uv[k];
				view.transform_tiny(UV,p[k]);
				uv.x			= (1+UV.x)*.5f;
				uv.y			= (1-UV.y)*.5f;
			}
			wm.m_Faces.push_back(F);
		}
	}
}

#if defined(USE_DX10) || defined(USE_DX11)
void CSkeletonX::_DuplicateIndices(const char* N, IReader *data)
{
	//	We will have trouble with container since don't know were to take readable indices
	VERIFY(!data->find_chunk(OGF_ICONTAINER));
	//	Index buffer replica since we can't read from index buffer in DX10
	//ref_smem<u16>			Indices;
	R_ASSERT			(data->find_chunk(OGF_INDICES));
	u32 iCount			= data->r_u32();

	u32 size				= iCount*2;
	u32 crc					= crc32( data->pointer(), size);
	m_Indices.create		( crc, iCount, (u16*)data->pointer());
}
#endif	//	USE_DX10