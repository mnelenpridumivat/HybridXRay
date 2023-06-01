#include "stdafx.h"

UIRightBarForm::UIRightBarForm() {}

UIRightBarForm::~UIRightBarForm() {}

void UIRightBarForm::Draw()
{
    ImGui::Begin("RightBar", 0);
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::CollapsingHeader(("  Custom Object"_RU >> u8"  Пользовательский объект"), /*ImGuiTreeNodeFlags_DefaultOpen |*/ ImGuiTreeNodeFlags_Framed | ImGuiMouseCursor_Hand))
    {
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "", ImGuiStyleVar_WindowTitleAlign);
        if (ImGui::BeginChild("Custom Object"_RU >> u8"Пользовательский объект", ImVec2(0, 166), true, ImGuiStyleVar_WindowTitleAlign | ImGuiMouseCursor_Hand))
        {
            ImGui::Dummy(ImVec2(0, 1));
            ImGui::AlignTextToFramePadding();
            ImGui::BeginGroup();
            PTools->m_ObjectProps->Draw();
            ImGui::EndGroup();
            ImGui::EndChild();
        }
    }
    ImGui::Separator();
    // ------------------------------------------------------------------------------------------------------ //
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(("  Item Properties"_RU >> u8"  Свойства"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiMouseCursor_Hand))
    {
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "", ImGuiStyleVar_WindowTitleAlign);
        if (ImGui::BeginChild("Item Properties"_RU >> u8"Свойства", ImVec2(0, 0), true, ImGuiStyleVar_WindowTitleAlign | ImGuiMouseCursor_Hand))
        {
            ImGui::Dummy(ImVec2(0, 1));
            ImGui::AlignTextToFramePadding();
            ImGui::BeginGroup();
            PTools->m_ItemProps->Draw();
            ImGui::EndGroup();
            ImGui::EndChild();
        }
    }
    ImGui::End();
}
