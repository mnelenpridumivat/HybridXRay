#include "stdafx.h"

UILightTool::UILightTool() {}

UILightTool::~UILightTool() {}

void UILightTool::Draw()
{
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::CollapsingHeader(("  Affect in D3D"_RU >> u8"  Влияние D3D"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginGroup();
        ImGui::PushItemWidth(-1);
        float size = float(ImGui::CalcItemWidth());
        {
            if (ImGui::Button("Enable Sel"_RU >> u8"Включить", ImVec2(size / 2, 0)))
                UseInD3D(false, true);
            ImGui::SameLine(0, 2);
            if (ImGui::Button("Enable All"_RU >> u8"Включить всё", ImVec2(size / 2, 0)))
                UseInD3D(true, true);

            if (ImGui::Button("Disable Sel"_RU >> u8"Отключить", ImVec2(size / 2, 0)))
                UseInD3D(false, false);
            ImGui::SameLine(0, 2);
            if (ImGui::Button("Disable All"_RU >> u8"Отключить все", ImVec2(size / 2, 0)))
                UseInD3D(true, false);
        }
        ImGui::EndGroup();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

void UILightTool::UseInD3D(bool bAll, bool bFlag)
{
    ObjectIt _F = Scene->FirstObj(OBJCLASS_LIGHT);
    ObjectIt _E = Scene->LastObj(OBJCLASS_LIGHT);
    for (; _F != _E; _F++)
    {
        CLight* L = (CLight*)*_F;
        if (bAll)
        {
            L->AffectD3D(bFlag);
        }
        else
        {
            if (L->Selected() && L->Visible())
                L->AffectD3D(bFlag);
        }
    }
    UI->RedrawScene();
}
