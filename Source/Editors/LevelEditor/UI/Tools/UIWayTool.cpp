#include "stdafx.h"

UIWayTool::UIWayTool()
{
    m_WayMode  = true;
    m_AutoLink = true;
}

UIWayTool::~UIWayTool() {}

void UIWayTool::Draw()
{
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::TreeNode("Commands"_RU >> u8"Выбор Режима"))
    {
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
        {
            if (ImGui::RadioButton("Way Mode"_RU >> u8"Режим 'Путь'", m_WayMode))
            {
                LTools->SetTarget(OBJCLASS_WAY, 0);
                m_WayMode = true;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SameLine();
            if (ImGui::RadioButton("Way Point"_RU >> u8"Режим 'Точка'", m_WayMode == false))
            {
                LTools->SetTarget(OBJCLASS_WAY, 1);
                m_WayMode = false;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
        ImGui::Separator();
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::TreeNode("Link Command"_RU >> u8"Редактор связей"))
    {
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
        {
            if (ImGui::Checkbox("Auto Link"_RU >> u8"Авто линковка", &m_AutoLink))
            {
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::PushItemWidth(-1);
            float size = float(ImGui::CalcItemWidth());
            {
                if (ImGui::Button("Create 1-Link"_RU >> u8"Создать 1-связь", ImVec2(size / 2, 0)))
                {
                    if (m_WayMode)
                    {
                        ELog.DlgMsg(mtInformation, "Before editing enter Point Mode.");
                        return;
                    }
                    bool bRes = false;
                    ObjectList lst;
                    Scene->GetQueryObjects(lst, OBJCLASS_WAY, 1, 1, 0);
                    // remove links
                    for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                    {
                        ((CWayObject*)(*it))->RemoveLink();
                        bRes |= ((CWayObject*)(*it))->Add1Link();
                    }
                    if (bRes)
                        Scene->UndoSave();
                    ExecCommand(COMMAND_UPDATE_PROPERTIES);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SameLine(0, 2);
                if (ImGui::Button("Convert to 1-Link"_RU >> u8"Конверт. в 1-связь", ImVec2(size / 2, 0)))
                {
                    ObjectList lst;
                    int cnt = Scene->GetQueryObjects(lst, OBJCLASS_WAY, 1, 1, 0);
                    for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                        ((CWayObject*)(*it))->Convert1Link();
                    if (cnt)
                        Scene->UndoSave();
                    ExecCommand(COMMAND_UPDATE_PROPERTIES);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

                if (ImGui::Button("Create 2-Link"_RU >> u8"Создать 2-связь", ImVec2(size / 2, 0)))
                {
                    if (m_WayMode)
                    {
                        ELog.DlgMsg(mtInformation, "Before editing enter Point Mode.");
                        return;
                    }
                    bool bRes = false;
                    ObjectList lst;
                    Scene->GetQueryObjects(lst, OBJCLASS_WAY, 1, 1, 0);
                    for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                        bRes |= ((CWayObject*)(*it))->Add2Link();
                    if (bRes)
                        Scene->UndoSave();
                    ExecCommand(COMMAND_UPDATE_PROPERTIES);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SameLine(0, 2);
                if (ImGui::Button("Convert to 2-Link"_RU >> u8"Конверт. в 2-связь", ImVec2(size / 2, 0)))
                {
                    ObjectList lst;
                    int cnt = Scene->GetQueryObjects(lst, OBJCLASS_WAY, 1, 1, 0);
                    for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                        ((CWayObject*)(*it))->Convert2Link();
                    if (cnt)
                        Scene->UndoSave();
                    ExecCommand(COMMAND_UPDATE_PROPERTIES);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

                if (ImGui::Button("Invert Link"_RU >> u8"Инвертировать связь", ImVec2(size / 2, 0)))
                {
                    if (m_WayMode)
                    {
                        ELog.DlgMsg(mtInformation, "Before editing enter Point Mode.");
                        return;
                    }
                    ObjectList lst;
                    int cnt = Scene->GetQueryObjects(lst, OBJCLASS_WAY, 1, 1, 0);
                    for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                        ((CWayObject*)(*it))->InvertLink();
                    if (cnt)
                        Scene->UndoSave();
                    ExecCommand(COMMAND_UPDATE_PROPERTIES);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SameLine(0, 2);
                if (ImGui::Button("Remove Link"_RU >> u8"Удалить связь", ImVec2(size / 2, 0)))
                {
                    if (m_WayMode)
                    {
                        ELog.DlgMsg(mtInformation, "Before editing enter Point Mode.");
                        return;
                    }
                    ObjectList lst;
                    int cnt = Scene->GetQueryObjects(lst, OBJCLASS_WAY, 1, 1, 0);
                    for (ObjectIt it = lst.begin(); it != lst.end(); it++)
                        ((CWayObject*)(*it))->RemoveLink();
                    if (cnt)
                        Scene->UndoSave();
                    ExecCommand(COMMAND_UPDATE_PROPERTIES);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
        }
        ImGui::Separator();
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
}
