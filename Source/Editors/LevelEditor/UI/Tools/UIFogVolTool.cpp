#include "stdafx.h"
UIFogVolTool::UIFogVolTool() {}

UIFogVolTool::~UIFogVolTool() {}

void UIFogVolTool::Draw()
{
    if (ImGui::CollapsingHeader(("  Commands"_RU >> u8"  Управление"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginGroup();
        ImGui::PushItemWidth(-1);
        float size = float(ImGui::CalcItemWidth());
        {
            if (ImGui::Button("Group Selected"_RU >> u8"Группировать", ImVec2(size / 2, 0)))
                ParentTools->GroupSelected();
            ImGui::SameLine(0, 2);
            if (ImGui::Button("UnGroup Selected"_RU >> u8"Разгруппировать", ImVec2(size / 2, 0)))
                ParentTools->UnGroupCurrent();
        }
        ImGui::EndGroup();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}
