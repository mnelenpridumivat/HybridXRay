#include "stdafx.h"
UIDOTool::UIDOTool()
{
    m_DOShuffle = false;
}

UIDOTool::~UIDOTool() {}

void UIDOTool::Draw()
{
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::TreeNode("Commands"_RU >> u8"Редактор Растительности"))
    {
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
        {
            if (ImGui::Button("First Initialize"_RU >> u8"Первая сборка", ImVec2(-1, 0)))
            {
                if (DM->Initialize())
                    Scene->UndoSave();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::Button("Reinitialize All"_RU >> u8"Пересобрать всё", ImVec2(-1, 0)))
            {
                if (DM->Reinitialize())
                    Scene->UndoSave();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::Button("Reinitialize Objects Only"_RU >> u8"Пересобрать одиночные", ImVec2(-1, 0)))
            {
                if (DM->UpdateObjects(true, false))
                    Scene->UndoSave();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::Button("Reinitialize Selected Slot Objects"_RU >> u8"Пересобрать в таблице", ImVec2(-1, 0)))
            {
                if (DM->UpdateObjects(false, true))
                    Scene->UndoSave();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::Button("Update Renderer"_RU >> u8"Обновить", ImVec2(-1, 0)))
            {
                DM->InvalidateCache();
                Scene->UndoSave();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::Button("Clear Slots"_RU >> u8"Очистить таблицу", ImVec2(-1, 0)))
            {
                if (ELog.DlgMsg(mtConfirmation, mbYes | mbNo, "Are you sure to reset slots?") == mrYes)
                {
                    DM->ClearSlots();
                    Scene->UndoSave();
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (ImGui::Button("Clear Details"_RU >> u8"Очистить в сцене", ImVec2(-1, 0)))
            {
                if (ELog.DlgMsg(mtConfirmation, mbYes | mbNo, "Are you sure to clear details?") == mrYes)
                {
                    ExecCommand(COMMAND_UPDATE_PROPERTIES);
                    DM->Clear();
                    Scene->UndoSave();
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Separator();
            if (ImGui::Button("Object List"_RU >> u8"Открыть таблицу", ImVec2(-1, 0)))
            {
                m_DOShuffle = true;
                UIDOShuffle::Show(DM);
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

void UIDOTool::OnDrawUI()
{
    if (m_DOShuffle)
    {
        if (UIDOShuffle::GetResult())
        {
            m_DOShuffle = false;
        }
        UIDOShuffle::Update();
    }
}
