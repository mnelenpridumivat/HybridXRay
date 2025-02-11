﻿#include "stdafx.h"

UIObjectList* UIObjectList::Form = nullptr;
UIObjectList::UIObjectList(): m_Root("")
{
    m_Mode      = M_Visible;
    m_Filter[0] = 0;
}

UIObjectList::~UIObjectList() {}

void UIObjectList::Draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(400, 400));

    if (!ImGui::Begin("Object List"_RU >> u8"Список Объектов", &bOpen))
    {
        ImGui::PopStyleVar(1);
        ImGui::End();
        return;
    }
    {
        ImGui::BeginGroup();
        DrawObjects();

        ImGui::SetNextItemWidth(-130);
        if (ImGui::InputText("##value", m_Filter, sizeof(m_Filter)))
        {
            m_Root.ClearSelcted();
        }
        ImGui::EndGroup();
    }
    ImGui::SameLine();
    if (ImGui::BeginChild("Right", ImVec2(130, 0)))
    {
        if (ImGui::RadioButton("All"_RU >> u8"ВСЕ", m_Mode == M_All))
        {
            m_Mode = M_All;
            m_Root.ClearSelcted();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::RadioButton("Visible Only"_RU >> u8"Только видимые", m_Mode == M_Visible))
        {
            m_Mode = M_Visible;
            m_Root.ClearSelcted();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::RadioButton("Invisible Only"_RU >> u8"Только скрытые", m_Mode == M_Inbvisible))
        {
            m_Mode = M_Inbvisible;
            m_Root.ClearSelcted();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::Separator();
        if (ImGui::Button("Show Selected"_RU >> u8"Показать выбранное", ImVec2(-1, 0)))
        {
            for (UITreeItem* Item: m_Root.Items)
            {
                UIObjectListItem* RItem = (UIObjectListItem*)Item;
                if (RItem->bIsSelected)
                {
                    RItem->Object->Show(true);
                }
            }
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

        if (ImGui::Button("Hide Selected"_RU >> u8"Скрыть выбранное", ImVec2(-1, 0)))
        {
            for (UITreeItem* Item: m_Root.Items)
            {
                UIObjectListItem* RItem = (UIObjectListItem*)Item;
                if (RItem->bIsSelected)
                {
                    RItem->Object->Show(false);
                }
            }
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

        ImGui::Separator();
        if (ImGui::Button("Focus on Selected"_RU >> u8"Фокус на выбранном", ImVec2(-1, 0)))
        {
            for (UITreeItem* Item: m_Root.Items)
            {
                UIObjectListItem* RItem = (UIObjectListItem*)Item;
                if (RItem->bIsSelected)
                {
                    RItem->Object->Select(true);

                    Fbox bb;
                    if (RItem->Object->GetBox(bb))
                        EDevice->m_Camera.ZoomExtents(bb);
                }
            }
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
    ImGui::EndChild();

    ImGui::PopStyleVar(1);
    ImGui::End();
}

void UIObjectList::Update()
{
    if (Form)
    {
        if (!Form->IsClosed())
        {
            Form->Draw();
        }
        else
        {
            xr_delete(Form);
        }
    }
}

void UIObjectList::Show()
{
    if (Form == nullptr)
        Form = xr_new<UIObjectList>();
    Refresh();
}

void UIObjectList::Close()
{
    xr_delete(Form);
}

void UIObjectList::Refresh()
{
    if (Form == nullptr)
        return;
    Form->m_Root    = UIObjectListItem("");

    Form->m_cur_cls = LTools->CurrentClassID();
    for (SceneToolsMapPairIt it = Scene->FirstTool(); it != Scene->LastTool(); ++it)
    {
        ESceneCustomOTool* ot = dynamic_cast<ESceneCustomOTool*>(it->second);
        if (ot && ((Form->m_cur_cls == OBJCLASS_DUMMY) || (it->first == Form->m_cur_cls)))
        {
            if (it->first == OBJCLASS_DUMMY)
                continue;
            ObjectList& lst   = ot->GetObjects();
            size_t      Index = 0;
            for (CCustomObject* Obj: lst)
            {
                if (Obj->GetName() == 0 || Obj->GetName()[0] == 0)
                {
                    continue;
                }
                else
                {
                    UIObjectListItem* Item = static_cast<UIObjectListItem*>(Form->m_Root.AppendItem(Obj->GetName(), {}, 0));
                    VERIFY(Item);
                    Item->Object = Obj;
                }
            }
        }
    }
    Form->m_LastSelected = nullptr;
}

void UIObjectList::DrawObjects()
{
    if (LTools->CurrentClassID() != m_cur_cls)
        Refresh();

    static ImGuiTableFlags flags = 
        ImGuiTableFlags_BordersV
        | ImGuiTableFlags_BordersOuterH
        | ImGuiTableFlags_Resizable
        | ImGuiTableFlags_RowBg
        | ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_ScrollY;

    if (ImGui::BeginTable("objects", 1, flags, ImVec2(-130, -ImGui::GetFrameHeight() - 4)))
    {
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableSetupColumn("Objects"_RU >> u8"Объекты", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        m_Root.DrawRoot();
        ImGui::EndTable();
    }
}
