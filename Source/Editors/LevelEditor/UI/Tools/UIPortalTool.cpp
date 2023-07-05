#include "stdafx.h"
UIPortalTool::UIPortalTool() {}

UIPortalTool::~UIPortalTool() {}

void UIPortalTool::Draw()
{
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::CollapsingHeader(("  Commands"_RU >> u8"  Управление"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginGroup();
        {
            if (ImGui::Button("Invert Orientation"_RU >> u8"Инвертировать", ImVec2(-1, 0)))
            {
                ObjectList lst;
                if (Scene->GetQueryObjects(lst, OBJCLASS_PORTAL, 1, 1, 0))
                {
                    for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                    {
                        CPortal* _O = (CPortal*)*it;
                        _O->InvertOrientation(true);
                    }
                }
            }
            if (ImGui::Button("Compute All Portals"_RU >> u8"Компилировать все порталы", ImVec2(-1, 0)))
            {
                if (mrYes ==
                    ELog.DlgMsg(mtConfirmation, mbYes | mbNo, "Are you sure want to destroy all existing portals and compute them again?"))
                {
                    int cnt = PortalUtils.CalculateAllPortals();
                    if (cnt)
                        ELog.DlgMsg(mtInformation, "Calculated '%d' portal(s).", cnt);
                }
            }
            if (ImGui::Button("Compute Sel. Portals"_RU >> u8"Компилировать выбранное", ImVec2(-1, 0)))
            {
                if (mrYes == ELog.DlgMsg(mtConfirmation, mbYes | mbNo, "Are you sure want to destroy all existing portals and compute them again?"))
                {
                    int cnt = PortalUtils.CalculateSelectedPortals();
                    if (cnt)
                        ELog.DlgMsg(mtInformation, "Calculated '%d' portal(s).", cnt);
                }
            }
            if (ImGui::Button("Remove Similar"_RU >> u8"Очистить", ImVec2(-1, 0)))
            {
                tool->RemoveSimilar();
            }
        }
        ImGui::Separator();
        ImGui::EndGroup();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}
