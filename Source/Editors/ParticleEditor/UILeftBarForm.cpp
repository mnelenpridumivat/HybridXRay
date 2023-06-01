#include "stdafx.h"

UILeftBarForm::UILeftBarForm() {}

UILeftBarForm::~UILeftBarForm() {}

void UILeftBarForm::Draw()
{
    ImGui::Begin("LeftBar", 0);
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::CollapsingHeader(("  Effect(s):"_RU >> u8"  Эффект(ы):"), /*ImGuiTreeNodeFlags_DefaultOpen |*/ ImGuiTreeNodeFlags_Framed | ImGuiMouseCursor_Hand))
    {
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "", ImGuiStyleVar_WindowTitleAlign);
        if (ImGui::BeginChild("Effect(s):"_RU >> u8"Эффект(ы):", ImVec2(0, 150), true, ImGuiStyleVar_WindowTitleAlign | ImGuiMouseCursor_Hand))
        {
            ImGui::Dummy(ImVec2(0, 1));
            ImGui::AlignTextToFramePadding();
            ImGui::BeginGroup();
            PTools->DrawReferenceList();
            ImGui::EndGroup();
            // if (ImGui::IsItemHovered())
            //    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::EndChild();
        }
    }
    ImGui::Separator();
    // ------------------------------------------------------------------------------------------------------ //
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(("  Particles:"_RU >> u8"  Партиклы:"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiMouseCursor_Hand))
    {
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "", ImGuiStyleVar_WindowTitleAlign);
        if (ImGui::BeginChild("Particles:"_RU >> u8"Партиклы:", ImVec2(0, 0), true, ImGuiStyleVar_WindowTitleAlign | ImGuiMouseCursor_Hand))
        {
            ImGui::Dummy(ImVec2(0, 1));
            ImGui::AlignTextToFramePadding();
            ImGui::BeginGroup();
            PTools->m_PList->Draw();
            ImGui::EndGroup();
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::EndChild();
        }
    }
    ImGui::End();
}
