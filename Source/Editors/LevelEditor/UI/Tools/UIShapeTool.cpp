#include "stdafx.h"

UIShapeTool::UIShapeTool()
{
    Tool          = nullptr;
    m_AttachShape = false;
}

UIShapeTool::~UIShapeTool() {}

void UIShapeTool::Draw()
{
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::TreeNode("Select a view"_RU >> u8"Выбрать вид"))
    {
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
        {
            if (ImGui::RadioButton("Sphere"_RU >> u8"Сфера", m_SphereMode))
            {
                m_SphereMode = true;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Box"_RU >> u8"Куб", m_SphereMode == false))
            {
                m_SphereMode = false;
            }
        }
        ImGui::Separator();
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::TreeNode("Edit"_RU >> u8"Редактор"))
    {
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
        {
            if (ImGui::Checkbox("Attach Shape..."_RU >> u8"Присоединить Шейп...", &m_AttachShape))
            {
                if (m_AttachShape)
                    ExecCommand(COMMAND_CHANGE_ACTION, etaAdd);
            }
            ImGui::SameLine(0, 10);
            if (ImGui::Button("Detach All"_RU >> u8"Отсоединить всё", ImVec2(-1, 0)))
            {
                ObjectList lst;
                if (Scene->GetQueryObjects(lst, OBJCLASS_SHAPE, 1, 1, 0))
                {
                    Scene->SelectObjects(false, OBJCLASS_SHAPE);
                    for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                        ((CEditShape*)*it)->Detach();
                }
            }
        }
        ImGui::Separator();
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::TreeNode("Level Bound"_RU >> u8"Ограничение уровня"))
    {
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
        {
            if (ImGui::Checkbox("Edit Level Bound"_RU >> u8"Редактировать границу уровня", &EditLevelBound))
            {
                if (EditLevelBound)
                    Tool->OnEditLevelBounds(false);
            }
            if (EditLevelBound)
                if (ImGui::Button("Recalc"_RU >> u8"Пересчитать", ImVec2(-1, 0)))
                {
                    Tool->OnEditLevelBounds(true);
                }
        }
        ImGui::Separator();
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
}
