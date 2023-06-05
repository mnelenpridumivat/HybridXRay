#include "stdafx.h"
UIFogVolTool::UIFogVolTool() {}

UIFogVolTool::~UIFogVolTool() {}

void UIFogVolTool::Draw()
{
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::TreeNode("Commands"_RU >> u8"Управление:"))
    {
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::PushItemWidth(-1);
        float size = float(ImGui::CalcItemWidth());
        {
            if (ImGui::Button("Group Selected"_RU >> u8"Группировать", ImVec2(size / 2, 0)))
                ParentTools->GroupSelected();
            ImGui::SameLine(0, 2);
            if (ImGui::Button("UnGroup Selected"_RU >> u8"Разгруппировать", ImVec2(size / 2, 0)))
                ParentTools->UnGroupCurrent();
        }

        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
}
