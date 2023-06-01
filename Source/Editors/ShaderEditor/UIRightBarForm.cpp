#include "stdafx.h"

UIRightBarForm::UIRightBarForm() {}

UIRightBarForm::~UIRightBarForm() {}

void UIRightBarForm::Draw()
{
    ImGui::Begin("RightBar", 0);
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    // ------------------------------------------------------------------------------------------------------ //
    if (!STools->m_PreviewProps->Empty())
    {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader(("  Preview"_RU >> u8"  Предпросмотр"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiMouseCursor_Hand))
        {
            ImGui::SameLine(0, 10);
            ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "", ImGuiStyleVar_WindowTitleAlign);
            if (ImGui::BeginChild("Preview"_RU >> u8"Предпросмотр", ImVec2(0, 100), true, ImGuiStyleVar_WindowTitleAlign | ImGuiMouseCursor_Hand))
            {
                ImGui::Dummy(ImVec2(0, 1));
                ImGui::AlignTextToFramePadding();
                ImGui::BeginGroup();
                STools->m_PreviewProps->Draw();
                ImGui::EndGroup();
                ImGui::EndChild();
            }
        }
    }
    // ------------------------------------------------------------------------------------------------------ //
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(("  Item Properties"_RU >> u8"  Параметры объекта"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiMouseCursor_Hand))
    {
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "", ImGuiStyleVar_WindowTitleAlign);
        if (ImGui::BeginChild("Item Properties"_RU >> u8"Параметры объекта", ImVec2(0, 0), true, ImGuiStyleVar_WindowTitleAlign | ImGuiMouseCursor_Hand))
        {
            ImGui::Dummy(ImVec2(0, 1));
            ImGui::AlignTextToFramePadding();
            ImGui::BeginGroup();
            STools->m_ItemProps->Draw();
            ImGui::EndGroup();
            ImGui::EndChild();
        }
    }
    ImGui::End();
}
