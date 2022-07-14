//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "UI_ActorTools.h"
#include "../xrECore/Editor/UI_Main.h"
#include "../xrECore/Editor/EditObject.h"
#include "motion.h"
#include "bone.h"
#include "SkeletonAnimated.h"
#include "fmesh.h"
//---------------------------------------------------------------------------
MotionID EngineModel::FindMotionID(LPCSTR name, u16 slot)
{
	MotionID M;
	CKinematicsAnimated* VA = PKinematicsAnimated(m_pVisual);
    if (VA) M				= VA->ID_Motion	(name,slot);
    return M;
}
CMotionDef*	EngineModel::FindMotionDef(LPCSTR name, u16 slot)
{
	CKinematicsAnimated* VA 	= PKinematicsAnimated(m_pVisual);
    if (VA){
        MotionID M				= FindMotionID(name,slot);
        if (M.valid())			return VA->LL_GetMotionDef(M);
    }
    return 0;
}
CMotion*	EngineModel::FindMotionKeys(LPCSTR name, u16 slot)
{
	CKinematicsAnimated* VA 	= PKinematicsAnimated(m_pVisual);
    if (VA){
    	MotionID M				= FindMotionID(name,slot);
        if (M.valid())			return VA->LL_GetMotion	(M,VA->LL_GetBoneRoot());
    }
    return 0;
}

void EngineModel::FillMotionList(LPCSTR pref, ListItemsVec& items, int modeID)
{
}
/*
void EngineModel::PlayCycle(LPCSTR name, int part, u16 slot)
{
    MotionID D = PKinematicsAnimated(m_pVisual)->ID_Motion(name,slot);
    if (D.valid())
        PKinematicsAnimated(m_pVisual)->LL_PlayCycle((u16)part,D,TRUE,0,0);
}

void EngineModel::PlayFX(LPCSTR name, float power, u16 slot)
{
    MotionID D = PKinematicsAnimated(m_pVisual)->ID_Motion(name,slot);
    if (D.valid())
    	PKinematicsAnimated(m_pVisual)->PlayFX(D,power);
}
*/

void EngineModel::StopAnimation()
{
    if (m_pVisual &&  PKinematicsAnimated(m_pVisual) ){
        PKinematicsAnimated(m_pVisual)->LL_CloseCycle(0);
        PKinematicsAnimated(m_pVisual)->LL_CloseCycle(1);
        PKinematicsAnimated(m_pVisual)->LL_CloseCycle(2);
        PKinematicsAnimated(m_pVisual)->LL_CloseCycle(3);
    }
}

bool EngineModel::UpdateGeometryStream(CEditableObject* source)
{
	m_GeometryStream.clear();
    if (!source) return false;
    if (source->IsSkeleton())	return (source->PrepareSVGeometry(m_GeometryStream,4));
    else						return (source->PrepareOGF(m_GeometryStream,4,true,NULL));
}

bool EngineModel::UpdateMotionDefsStream(CEditableObject* source)
{
	m_MotionDefsStream.clear();
	return (source&&source->PrepareSVDefs(m_MotionDefsStream));
}

bool EngineModel::UpdateMotionKeysStream(CEditableObject* source)
{
	m_MotionKeysStream.clear();
	return (source&&source->PrepareSVKeys(m_MotionKeysStream));
}

bool EngineModel::UpdateVisual(CEditableObject* source, bool bUpdGeom, bool bUpdKeys, bool bUpdDefs)
{
	bool bRes = true;
    return bRes;
}

//---------------------------------------------------------------------------

void EngineModel::PlayMotion(LPCSTR name, u16 slot)
{
    for (int k=0; k<MAX_PARTS; k++)
       m_BPPlayItems[k].name = "";

    StopAnimation();

    CKinematicsAnimated* SA 	= PKinematicsAnimated(m_pVisual);
	if (IsRenderable()&&SA){
        MotionID motion_ID 		= FindMotionID(name, slot);
        if (motion_ID.valid()){
            CMotionDef* mdef 	= SA->LL_GetMotionDef(motion_ID); VERIFY(mdef);
            if (mdef->flags&esmFX)
            {
                for (int k=0; k<MAX_PARTS; k++)
                {
                    if (!m_BPPlayItems[k].name.empty())
                    {
                        MotionID D 		= SA->ID_Motion(m_BPPlayItems[k].name.c_str(),m_BPPlayItems[k].slot);
                        if (D.valid()) 	SA->LL_PlayCycle((u16)k,D,false,0,0);
                    }
                }        
                m_pBlend = SA->PlayFX(motion_ID,1.f);
            }else{	
                u16 idx 		= mdef->bone_or_part;
                R_ASSERT((idx==BI_NONE)||(idx<MAX_PARTS));
                if (BI_NONE==idx)
                {
                	for (int k=0; k<MAX_PARTS; k++){ 
                		m_BPPlayItems[k].name 	= name;
	                    m_BPPlayItems[k].slot	= slot;
                    }
                }else{	
	                m_BPPlayItems[idx].name		= name;
					m_BPPlayItems[idx].slot		= slot;
                }
                m_pBlend		= 0;

                for (int k=0; k<MAX_PARTS; k++)
                {
                    if (!m_BPPlayItems[k].name.empty())
                    {
                        MotionID D 	= SA->ID_Motion(m_BPPlayItems[k].name.c_str(),m_BPPlayItems[k].slot);
                        CBlend* B	= 0;
                        if (D.valid())
                        {
                            B = SA->LL_PlayCycle((u16)k,D,false,0,0);
                            if(B && (idx==k || idx==BI_NONE) ) 
                            	m_pBlend = B;
                        }
                    }
                }        
            }
        }
    }
/*
    if (M&&IsRenderable()){
        if (M->flags&esmFX){
			for (int k=0; k<MAX_PARTS; k++){
            	if (!m_BPPlayCache[k].IsEmpty()){
                	CMotionDef* D = PSkeletonAnimated(m_pVisual)->ID_Cycle_Safe(m_BPPlayCache[k].c_str());
                    if (D) D->PlayCycle(PSkeletonAnimated(m_pVisual),k,false,0,0);
    	    	}
            }        
        	m_pBlend = PSkeletonAnimated(m_pVisual)->PlayFX(M->Name(),1.f);
        }else{	
        	R_ASSERT((M->m_BoneOrPart==BI_NONE)||(M->m_BoneOrPart<MAX_PARTS));
            u16 idx 		= M->m_BoneOrPart;
        	if (BI_NONE==idx)for (int k=0; k<MAX_PARTS; k++) m_BPPlayCache[k] = M->Name();
            else			m_BPPlayCache[idx] = M->Name();
            m_pBlend		= 0;

			for (int k=0; k<MAX_PARTS; k++){
            	if (!m_BPPlayCache[k].IsEmpty()){
                	CMotionDef* D = PSkeletonAnimated(m_pVisual)->ID_Cycle_Safe(m_BPPlayCache[k].c_str());
                    CBlend* B=0;
                    if (D){
                    	B = D->PlayCycle(PSkeletonAnimated(m_pVisual),k,(idx==k)?!(D->flags&esmNoMix):FALSE,0,0);
						if (idx==k) m_pBlend = B;
                    }
    	    	}
            }        
        }
    }
*/
}/*
void EngineModel::RestoreParams(TFormStorage* s)
{          
    for (u16 k=0; k<MAX_PARTS; k++){
    	m_BPPlayItems[k].name	= s->ReadString("bp_cache_name_"+AnsiString(k),"");
    	m_BPPlayItems[k].slot	= (u16)s->ReadInteger("bp_cache_slot_"+AnsiString(k),0);
    }
}

void EngineModel::SaveParams(TFormStorage* s)
{
    for (int k=0; k<MAX_PARTS; k++){
	    s->WriteString	("bp_cache_name_"+AnsiString(k),	m_BPPlayItems[k].name);
	    s->WriteString	("bp_cache_slot_"+AnsiString(k),	m_BPPlayItems[k].slot);
    }
}*/

//---------------------------------------------------------------------------

void CActorTools::OnMotionKeysModified()
{
}

void CActorTools::OnMotionDefsModified()
{
}

void CActorTools::OnGeometryModified()
{
}
//---------------------------------------------------------------------------

bool CActorTools::AppendMotion(LPCSTR fn)
{
	VERIFY(m_pEditObject);

    m_pEditObject->m_SMotionRefs.clear();
    bool bRes = m_pEditObject->AppendSMotion(fn,&appended_motions);
    return bRes;
}

bool CActorTools::RemoveMotion(LPCSTR name)
{
	VERIFY(m_pEditObject);
    return m_pEditObject->RemoveSMotion(name);
}

bool CActorTools::SaveMotions(LPCSTR name, bool bSelOnly)
{
	VERIFY(m_pEditObject);
    if (bSelOnly)
    {
        return m_pEditObject->SaveSMotion(name);
    }
    else
    {
    	return m_pEditObject->SaveSMotions(name);
    }
    return false;
}

bool CActorTools::LoadBoneData(shared_str file)
{
    xr_string full_name = file.c_str();

    if (FS.exist(full_name.c_str()))
    {
        IReader* R = FS.r_open(full_name.c_str());
        return m_pEditObject->LoadBoneData(*R);
    }
    return false;
}

bool CActorTools::SaveBoneData(shared_str file)
{
    IWriter* W = FS.w_open(file.c_str());
    if (W)
    {
        m_pEditObject->SaveBoneData(*W);
        FS.w_close(W);
        return true;
    }
    return false;
}

void CActorTools::MakePreview()
{
}

void CActorTools::PlayMotion()
{
}

void CActorTools::StopMotion()
{
}

void CActorTools::PauseMotion()
{
}

bool CActorTools::RenameMotion(LPCSTR old_name, LPCSTR new_name)
{
	R_ASSERT(m_pEditObject);
	CSMotion* M = m_pEditObject->FindSMotionByName(old_name);	R_ASSERT(M);
	CSMotion* MN = m_pEditObject->FindSMotionByName(new_name);	R_ASSERT(!MN);
    M->SetName(new_name);
    return true;
}


void CActorTools::AddMarksChannel(bool b12)
{
    CSMotion* M 		= m_pEditObject->GetActiveSMotion();
    if(M)
    {
    	if(b12)
        {
            M->marks.resize					(2);
            M->marks[0].name				= "Left";
            M->marks[1].name				= "Right";
		}else
        {
            M->marks.resize					(4);
            M->marks[0].name				= "Left";
            M->marks[1].name				= "Right";
            M->marks[2].name				= "Left2";
            M->marks[3].name				= "Right2";
        }
        m_pEditObject->SetActiveSMotion(M);
    }
}

void CActorTools::RemoveMarksChannel(bool b12)
{
    CSMotion* M 					= m_pEditObject->GetActiveSMotion();
    if(M)
    {
        if(b12)
        	M->marks.clear			();
        else
        {
            if(M->marks.size()==4)
            {
                M->marks.pop_back();
                M->marks.pop_back();
            }else
            {
            	R_ASSERT(M->marks.size()==0 || M->marks.size()==2);
            }
        }

        ExecCommand					(COMMAND_UPDATE_PROPERTIES);
    }
}
