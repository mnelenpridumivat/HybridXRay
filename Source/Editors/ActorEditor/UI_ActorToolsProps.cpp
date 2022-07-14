//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop
#include "..\..\XrRender\Private\KinematicAnimatedDefs.h"
#include "SkeletonAnimated.h"
//------------------------------------------------------------------------------

void  CActorTools::OnObjectItemsFocused(xr_vector<ListItem*>& items)
{
}
//------------------------------------------------------------------------------

void CActorTools::OnChangeTransform(PropValue* sender)
{
    OnMotionKeysModified();
	UI->RedrawScene();
}

void CActorTools::RealUpdateProperties()
{
}
//------------------------------------------------------------------------------
                               
void CActorTools::OnMotionTypeChange(PropValue* sender)
{
	RefreshSubProperties();
}
//------------------------------------------------------------------------------
                        
void CActorTools::OnMotionNameChange(PropValue* V)
{
    OnMotionKeysModified();
    UpdateProperties();
}

//------------------------------------------------------------------------------

void  CActorTools::OnMotionRefsChange(PropValue* sender)
{
    u32 set_cnt		= _GetItemCount(m_tmp_mot_refs.c_str());

    m_pEditObject->m_SMotionRefs.clear();
    m_pEditObject->m_SMotionRefs.reserve(set_cnt);

    string_path		nm;
    for (u32 k=0; k<set_cnt; ++k)
    {
        _GetItem						(m_tmp_mot_refs.c_str(),k,nm);
        m_pEditObject->m_SMotionRefs.push_back (nm);
    }

    OnMotionKeysModified	();
	ExecCommand				(COMMAND_UPDATE_PROPERTIES);
}

#include "envelope.h"
Fvector StartMotionPoint, EndMotionPoint;
void CActorTools::FillMotionProperties(PropItemVec& items, LPCSTR pref, ListItem* sender)
{
}
//------------------------------------------------------------------------------

xr_token joint_types[]={
	{ "Rigid",					jtRigid		},
	{ "Cloth",					jtCloth		},
	{ "Joint",					jtJoint	 	},
	{ "Wheel [Steer-X/Roll-Z]", jtWheel		},
	{ "Slider",					jtSlider 	},
//	{ "Wheel [Steer-X/Roll-Z]", jtWheelXZ	},
//	{ "Wheel [Steer-X/Roll-Y]", jtWheelXY	},
//	{ "Wheel [Steer-Y/Roll-X]", jtWheelYX	},
//	{ "Wheel [Steer-Y/Roll-Z]", jtWheelYZ	},
//	{ "Wheel [Steer-Z/Roll-X]", jtWheelZX	},
//	{ "Wheel [Steer-Z/Roll-Y]", jtWheelZY	},
	{ 0,						0			}
};

xr_token shape_types[]={
	{ "None",		SBoneShape::stNone		},
	{ "Box",		SBoneShape::stBox		},
	{ "Sphere",		SBoneShape::stSphere	},
	{ "Cylinder",	SBoneShape::stCylinder	},
	{ 0,			0						}
};

static const LPCSTR axis[3]=
{ "Axis X", "Axis Y", "Axis Z" };

void  CActorTools::OnJointTypeChange(PropValue* V)
{
	ExecCommand(COMMAND_UPDATE_PROPERTIES);
}
void  CActorTools::OnShapeTypeChange(PropValue* V)
{
	UI->RedrawScene();
	ExecCommand(COMMAND_UPDATE_PROPERTIES);
}
void  CActorTools::OnBindTransformChange(PropValue* V)
{
	R_ASSERT(m_pEditObject);
    m_pEditObject->OnBindTransformChange();
	UI->RedrawScene();
}

void CActorTools::OnTypeChange(PropValue* V)
{
    u32 current_type = m_pEditObject->m_objectFlags.flags & (CEditableObject::eoDynamic | CEditableObject::eoHOM | CEditableObject::eoSoundOccluder | CEditableObject::eoMultipleUsage);
    m_pEditObject->m_objectFlags.flags = m_pEditObjectType;
    
    if (current_type != m_pEditObjectType)
    {
        if (m_pEditObjectType == CEditableObject::eoMultipleUsage)
        {
            m_pEditObject->m_objectFlags.flags |= CEditableObject::eoUsingLOD;
        }
    }
    RefreshSubProperties();
}

void CActorTools::OnUsingLodFlagChange(PropValue* V)
{
    RefreshSubProperties();
}

void GetBindAbsolutePosition(CBone* B, Fmatrix& dest)
{

	if(B->Parent())
    	GetBindAbsolutePosition(B->Parent(), dest);

    {
    	Fmatrix 	M;
        M.setXYZi	( B->_RestRotate() );
        M.c.set		( B->_RestOffset() );
        M.mulA_43	(dest);
        dest.set	(M);
    }

}

void CActorTools::OnDrawUI()
{
}
bool CActorTools::OnBoneNameAfterEdit(PropValue* sender, shared_str& edit_val)
{
    R_ASSERT(m_pEditObject);
    BoneVec 		sel_bones;
    m_pEditObject->GetSelectedBones(sel_bones);
    CBone* B = sel_bones.size() ? sel_bones[0] : NULL;
    R_ASSERT(B);
    for (auto& bone : m_pEditObject->Bones())
    {
        if (bone->Name() == edit_val)
            return false;
    }
    m_pEditObject->RenameBone(B, edit_val.c_str());
    return true;
}

void CActorTools::OnBoneNameChangeEvent(PropValue* sender)
{
    UpdateProperties();
}

void  CActorTools::OnBoneLimitsChange(PropValue* sender)
{
	m_pEditObject->ClampByLimits(true);
}

void CActorTools::FillBoneProperties(PropItemVec& items, LPCSTR pref, ListItem* sender)
{
}
//------------------------------------------------------------------------------

void CActorTools::FillSurfaceProperties(PropItemVec& items, LPCSTR pref, ListItem* sender)
{
}
//------------------------------------------------------------------------------
xr_token eo_type_token[] = {
    { "Static",					0},
    { "Dynamic",				CEditableObject::eoDynamic},
    { "HOM",					CEditableObject::eoHOM},
    { "Multiple Usage",			CEditableObject::eoMultipleUsage},
    { "Sound Occluder",			CEditableObject::eoSoundOccluder},
    { 0,						0}
};

void CActorTools::FillObjectProperties(PropItemVec& items, LPCSTR pref, ListItem* sender)
{
}
//------------------------------------------------------------------------------

void CActorTools::SelectListItem(LPCSTR pref, LPCSTR name, bool bVal, bool bLeaveSel, bool bExpand)
{
}
//------------------------------------------------------------------------------

