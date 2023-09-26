// Originally by B.O.R.S.C.H.T. team
// see https://bitbucket.org/stalker/xray-csky_borscht_sdk

#include "stdafx.h"

#include "IM_Manipulator.h"
#include "../XrEUI/imgui.h"
#include "../XrEUI/ImGuizmo.h"
#include "../../scene/scene.h"
#include "../../UI_LevelTools.h"
#include "../../Entry/CustomObject.h"

IM_Manipulator imManipulator;

void IM_Manipulator::Render(float canvasX, float canvasY, float canvasWidth, float canvasHeight)
{
    ImGuizmo::SetRect(canvasX, canvasY, canvasWidth, canvasHeight);
    ImGuizmo::SetDrawlist();

    ESceneCustomOTool* tool = Scene->GetOTool(LTools->CurrentClassID());
    if (!tool)
        return;

    ObjectList lst;
    tool->GetQueryObjects(lst, TRUE, TRUE, FALSE);
    if (lst.size() < 1)
        return;

    const bool           IsCSParent   = Tools->GetSettings(etfCSParent);
    Fmatrix              ObjectMatrix = lst.front()->FTransformRP;
    Fmatrix              DeltaMatrix  = Fidentity;
    float*               Snap         = NULL;

    switch (LTools->GetAction())
    {
        case etaMove:
        {
            Snap = LTools->GetSettings(etfMSnap) ? &Tools->m_MoveSnap : NULL;

            const bool IsManipulated = ImGuizmo::Manipulate((float*)&Device->mView, (float*)&Device->mProject, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, (float*)&ObjectMatrix, (float*)&DeltaMatrix, Snap);

            if (IsManipulated)
            {
              for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                  (*it)->Move(DeltaMatrix.c);
            }
        }
        break;
        case etaRotate:
        {
            float   Angle = rad2deg(Tools->m_RotateSnapAngle);
            Fvector OriginalRotation;

            ObjectMatrix.getXYZ(OriginalRotation);
            Snap = LTools->GetSettings(etfASnap) ? &Angle : NULL;

            const bool IsManipulated = ImGuizmo::Manipulate((float*)&Device->mView, (float*)&Device->mProject, ImGuizmo::ROTATE, ImGuizmo::WORLD, (float*)&ObjectMatrix, (float*)&DeltaMatrix, Snap);

            if (IsManipulated)
            {
                Fvector DeltaXYZ;
                 
                DeltaMatrix.getXYZ(DeltaXYZ);

                for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                {
                  void (CCustomObject::*Handler) (Fvector&, float);

                  if (IsCSParent)
                    Handler = &CCustomObject::RotateParent;
                  else
                    Handler = &CCustomObject::RotateLocal;

                  (*it->*Handler)(Fvector().set(0, 0, 1), -DeltaXYZ.z);
                  (*it->*Handler)(Fvector().set(1, 0, 0), -DeltaXYZ.x);
                  (*it->*Handler)(Fvector().set(0, 1, 0), -DeltaXYZ.y);
                }

                UI->UpdateScene();
            }
        }
        break;
        case etaScale:
        {
            Fvector original_scale;
            original_scale.x = ObjectMatrix.i.magnitude();
            original_scale.y = ObjectMatrix.j.magnitude();
            original_scale.z = ObjectMatrix.k.magnitude();

            const bool IsManipulated = ImGuizmo::Manipulate((float*)&Device->mView, (float*)&Device->mProject, ImGuizmo::SCALE, ImGuizmo::WORLD, (float*)&ObjectMatrix);

            if (IsManipulated)
            {
                Fvector scale;
                scale.x = ObjectMatrix.i.magnitude() - original_scale.x;
                scale.y = ObjectMatrix.j.magnitude() - original_scale.y;
                scale.z = ObjectMatrix.k.magnitude() - original_scale.z;

                for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                    (*it)->Scale(scale);
            }
        }
        break;
    }

    if (ImGuizmo::IsUsing() && !m_active)
    {
        // activate
        m_active = true;
    }

    if (!ImGuizmo::IsUsing() && m_active)
    {
        // deactivate
        Scene->UndoSave();
        m_active = false;
    }
}
