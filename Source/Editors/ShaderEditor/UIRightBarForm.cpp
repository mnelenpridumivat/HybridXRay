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
        if (ImGui::TreeNode("Preview"))
        {
            STools->m_PreviewProps->Draw();
            ImGui::TreePop();
        }
    }
    // ------------------------------------------------------------------------------------------------------ //
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Item Properties"))
    {
        ImGui::BeginGroup();
        STools->m_ItemProps->Draw();
        ImGui::EndGroup();
        ImGui::TreePop();
    }
    ImGui::End();
}
