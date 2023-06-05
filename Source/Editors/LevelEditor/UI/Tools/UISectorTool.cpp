#include "stdafx.h"
UISectorTool::UISectorTool()
{
    m_Edit              = false;
    m_CreateNewMultiple = false;
    m_CreateNewSingle   = false;
    m_MeshAdd           = true;
    m_BoxPick           = false;
}

UISectorTool::~UISectorTool() {}
void UISectorTool::Draw()
{
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::TreeNode("Commands"_RU >> u8"Управление"))
    {
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
        {
            if (ImGui::Button("Validate Sectors"_RU >> u8"Проверка Секторов", ImVec2(-1, 0)))
            {
                PortalUtils.Validate(true);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::Button("Capture Volume"_RU >> u8"Захват уровней", ImVec2(-1, 0)))
            {
                CSector* S = PortalUtils.GetSelectedSector();
                if (S)
                {
                    S->CaptureInsideVolume();
                    Scene->UndoSave();
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::Button("Distribute Objects"_RU >> u8"Распределить объекты", ImVec2(-1, 0)))
            {
                CSector* S = PortalUtils.GetSelectedSector();
                if (S)
                {
                    S->DistributeInsideObjects();
                    Scene->UndoSave();
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::Button("Create Default"_RU >> u8"Создать базовый сектор", ImVec2(-1, 0)))
            {
                CCustomObject* O = Scene->FindObjectByName(DEFAULT_SECTOR_NAME, OBJCLASS_SECTOR);
                if (O)
                    ELog.DlgMsg(mtInformation, "Default sector already present. Remove this and try again.");
                else
                {
                    if (!PortalUtils.CreateDefaultSector())
                        ELog.DlgMsg(mtInformation, "Default can't created.");
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::Button("Remove Default"_RU >> u8"Удалить базовый сектор", ImVec2(-1, 0)))
            {
                if (!PortalUtils.RemoveDefaultSector())
                    ELog.DlgMsg(mtInformation, "Default sector not found.");
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
        ImGui::Separator();
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    if (m_Edit)
        ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (m_Edit && ImGui::TreeNode("Edit"_RU >> u8"Редактор"))
    {
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
        {
            ImGui::Separator();
            if (ImGui::Checkbox("Create New Single(From Mesh)"_RU >> u8"Создать одиночный(Для Меша)", &m_CreateNewSingle))
            {
                m_CreateNewMultiple = false;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::Checkbox("Create New Multiple(From Object)"_RU >> u8"Создать множественный(Для Объекта)", &m_CreateNewMultiple))
            {
                m_CreateNewSingle = false;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            ImGui::Separator();
            ImGui::Text("Meshes"_RU >> u8"Модель:");
            ImGui::SameLine();
            if (ImGui::RadioButton(" M+ "_RU >> u8" +Mеш ", m_MeshAdd))
            {
                m_MeshAdd = true;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SameLine();
            if (ImGui::RadioButton(" M- "_RU >> u8" -Mеш ", !m_MeshAdd))
            {
                m_MeshAdd = false;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::Checkbox("Box Pick"_RU >> u8"Выбрать кубические", &m_BoxPick))
                ;
            if (m_CreateNewSingle || m_CreateNewMultiple)
            {
                m_BoxPick = false;
            }
        }
        ImGui::Separator();
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
}
