#include "stdafx.h"

UIParticlesTool::UIParticlesTool()
{
    m_Current       = nullptr;
    m_ParticlesList = xr_new<UIItemListForm>();
    m_ParticlesList->SetOnItemFocusedEvent(TOnILItemFocused(this, &UIParticlesTool::OnItemFocused));
    ListItemsVec items;
    for (PS::PEDIt E = ::RImplementation.PSLibrary.FirstPED(); E != ::RImplementation.PSLibrary.LastPED(); E++)
    {
        ListItem* I = LHelper().CreateItem(items, *(*E)->m_Name, 0, 0, *E);
        I->SetIcon(1);
    }
    for (PS::PGDIt G = ::RImplementation.PSLibrary.FirstPGD(); G != ::RImplementation.PSLibrary.LastPGD(); G++)
    {
        ListItem* I = LHelper().CreateItem(items, *(*G)->m_Name, 0, 0, *G);
        I->SetIcon(2);
    }

    m_ParticlesList->AssignItems(items);
}

UIParticlesTool::~UIParticlesTool()
{
    xr_delete(m_ParticlesList);
}

void UIParticlesTool::Draw()
{
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::CollapsingHeader(("  Commands"_RU >> u8"  Управление Партиклов"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginGroup();
        {
            ImGui::Text("Ref's Select:   "_RU >> u8"Добавить:       ");
            ImGui::SameLine(0, 20);
            if (ImGui::Button(" + ", ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight())))
            {
                SelByRef(true);
            };
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SameLine(0, 8);
            if (ImGui::Button(" - ", ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight())))
            {
                SelByRef(false);
            };
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::Text("Selected:        "_RU >> u8"Действие:       ");
            ImGui::SameLine(0, 21);
            if (ImGui::ArrowButton(" play ", ImGuiDir_Right))
            {
                ObjectIt _F = Scene->FirstObj(OBJCLASS_PS);
                ObjectIt _E = Scene->LastObj(OBJCLASS_PS);
                for (; _F != _E; _F++)
                {
                    if ((*_F)->Visible() && (*_F)->Selected())
                        ((EParticlesObject*)(*_F))->Play();
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SameLine(0, 8);
            if (ImGui::Button("stop", ImVec2(0, ImGui::GetFrameHeight())))
            {
                ObjectIt _F = Scene->FirstObj(OBJCLASS_PS);
                ObjectIt _E = Scene->LastObj(OBJCLASS_PS);
                for (; _F != _E; _F++)
                {
                    if ((*_F)->Visible() && (*_F)->Selected())
                        ((EParticlesObject*)(*_F))->Stop();
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
        ImGui::Separator();
        ImGui::EndGroup();
    }
    // ------------------------------------------------------------------------------------------------------ //
    ImGui::Separator();
    ImGui::Text("Library:"_RU >> u8"Библиотека:", ImVec2(0.5f, 0.5f));
    ImGui::Separator();
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::CollapsingHeader(("  Particles"_RU >> u8"  Партиклы"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTableFlags_NoBordersInBody))
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SameLine(0, 10);
        ImGui::BulletTextColored(ImVec4(0.75, 1.5, 0, 0.85), "");
        ImGui::BeginGroup();
        {
            ImGui::Separator();
            m_ParticlesList->Draw();
            ImGui::Separator();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::EndGroup();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

void UIParticlesTool::SelByRef(bool flag)
{
    if (m_Current)
    {
        ObjectIt _F = Scene->FirstObj(OBJCLASS_PS);
        ObjectIt _E = Scene->LastObj(OBJCLASS_PS);
        for (; _F != _E; _F++)
        {
            if ((*_F)->Visible())
            {
                EParticlesObject* _O = (EParticlesObject*)(*_F);
                if (_O->RefCompare(m_Current))
                    _O->Select(flag);
            }
        }
    }
}

void UIParticlesTool::OnItemFocused(ListItem* item)
{
    m_Current = nullptr;
    if (item)
    {
        m_Current = item->Key();
    }
}
