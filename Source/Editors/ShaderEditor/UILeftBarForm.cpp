#include "stdafx.h"

UILeftBarForm::UILeftBarForm() {}

UILeftBarForm::~UILeftBarForm() {}

void UILeftBarForm::Draw()
{
    ImGui::Begin("LeftBar", 0);
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyScroll))
    {
        for (auto& tool : STools->m_Tools)
        {
            const char* name = tool.second->ToolsName();
            if (ImGui::BeginTabItem(name))
            {
                if (STools->m_Current != tool.second)
                {
                    STools->OnChangeEditor(tool.second);
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::Separator();
                // ------------------------------------------------------------------------------------------------------ //
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                if (ImGui::CollapsingHeader(("  Items:"_RU >> u8"  Items:"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiMouseCursor_Hand))
                {
                    ImGui::SameLine(0, 10);
                    ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "", ImGuiStyleVar_WindowTitleAlign);
                    if (ImGui::BeginChild("Items:"_RU >> u8"Items:", ImVec2(0, 0), true, ImGuiStyleVar_WindowTitleAlign | ImGuiMouseCursor_Hand))
                    {
                        ImGui::Dummy(ImVec2(0, 1));
                        ImGui::AlignTextToFramePadding();
                        ImGui::BeginGroup();
                        STools->m_Items->Draw();
                        ImGui::EndGroup();
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
                    ImGui::EndChild();
                }
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}
