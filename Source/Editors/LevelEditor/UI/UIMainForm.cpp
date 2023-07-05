#include "stdafx.h"
#include "..\XrECore\Editor\EditorChooseEvents.h"
#include "..\resources\splash.h"

UIMainForm* MainForm = nullptr;
UIMainForm::UIMainForm()
{
    EnableReceiveCommands();
    if (!ExecCommand(COMMAND_INITIALIZE, (u32)0, (u32)0))
    {
        FlushLog();
        exit(-1);
    }
    ExecCommand(COMMAND_UPDATE_GRID);
    ExecCommand(COMMAND_RENDER_FOCUS);
    FillChooseEvents();
    m_TopBar          = xr_new<UITopBarForm>();
    m_Render          = xr_new<UIRenderForm>();
    m_MainMenu        = xr_new<UIMainMenuForm>();
    m_LeftBar         = xr_new<UILeftBarForm>();
    m_Properties      = xr_new<UILPropertiesFrom>();
    m_WorldProperties = xr_new<UIWorldPropertiesFrom>();
    m_Render->SetContextMenuEvent(TOnRenderContextMenu(this, &UIMainForm::DrawContextMenu));
    m_Render->SetToolBarEvent(TOnRenderToolBar(this, &UIMainForm::DrawRenderToolBar));
    if (dynamic_cast<CLevelPreferences*>(EPrefs)->OpenObjectList)
    {
        UIObjectList::Show();
    }
    if (!dynamic_cast<CLevelPreferences*>(EPrefs)->OpenProperties)
    {
        m_Properties->Close();
    }
    if (!dynamic_cast<CLevelPreferences*>(EPrefs)->OpenWorldProperties)
    {
        m_WorldProperties->Close();
    }

    m_tMenu    = EDevice->Resources->_CreateTexture("ed\\bar\\menu");
    m_tSelect  = EDevice->Resources->_CreateTexture("ed\\bar\\select");
    m_tAdd     = EDevice->Resources->_CreateTexture("ed\\bar\\add");
    m_tMove    = EDevice->Resources->_CreateTexture("ed\\bar\\move");
    m_tScale   = EDevice->Resources->_CreateTexture("ed\\bar\\scale");
    m_tRotate  = EDevice->Resources->_CreateTexture("ed\\bar\\rotate");
    m_tNSnap   = EDevice->Resources->_CreateTexture("ed\\bar\\nsnap");
    m_tZoomSel = EDevice->Resources->_CreateTexture("ed\\bar\\zoomsel");

    m_tGrid      = EDevice->Resources->_CreateTexture("ed\\bar\\grid");
    m_tScaleGrid = EDevice->Resources->_CreateTexture("ed\\bar\\scale_grid");
    m_tAngle     = EDevice->Resources->_CreateTexture("ed\\bar\\angle");

    LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 0.1f);
    LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 0.1f);
    LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 5.625f);

    LTools->GetGimzo()->SwitchStep(Gizmo::EType::Move, true);
    LTools->GetGimzo()->SwitchStep(Gizmo::EType::Scale, true);
    LTools->GetGimzo()->SwitchStep(Gizmo::EType::Rotate, true);
}

UIMainForm::~UIMainForm()
{
    dynamic_cast<CLevelPreferences*>(EPrefs)->OpenProperties      = !m_Properties->IsClosed();
    dynamic_cast<CLevelPreferences*>(EPrefs)->OpenWorldProperties = !m_WorldProperties->IsClosed();
    dynamic_cast<CLevelPreferences*>(EPrefs)->OpenObjectList      = UIObjectList::IsOpen();
    ClearChooseEvents();
    xr_delete(m_WorldProperties);
    xr_delete(m_Properties);
    xr_delete(m_LeftBar);
    xr_delete(m_MainMenu);
    xr_delete(m_Render);
    xr_delete(m_TopBar);

    m_tMenu.destroy();
    m_tSelect.destroy();
    m_tAdd.destroy();
    m_tMove.destroy();
    m_tScale.destroy();
    m_tRotate.destroy();
    m_tNSnap.destroy();
    m_tZoomSel.destroy();
    m_tGrid.destroy();
    m_tScaleGrid.destroy();
    m_tAngle.destroy();

    ExecCommand(COMMAND_DESTROY, (u32)0, (u32)0);
}

void UIMainForm::Draw()
{
    m_MainMenu->Draw();
    m_TopBar->Draw();
    m_LeftBar->Draw();
    m_Properties->Draw();
    m_WorldProperties->Draw();
    if (bDemoImGui)
        ImGui::ShowDemoWindow(&bDemoImGui);
    m_Render->Draw();
    splash::hide();
}

bool UIMainForm::Frame()
{
    if (UI)
        return UI->Idle();
    return false;
}

void UIMainForm::DrawContextMenu()
{
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::BeginMenu("Edit"_RU >> u8"Правка"))
    {
        if (ImGui::MenuItem("Duplicate"_RU >> u8"Дублировать"))
        {
            ExecCommand(COMMAND_DUPLICATE);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::Separator();
        if (ImGui::MenuItem("Copy"_RU >> u8"Копировать"))
        {
            ExecCommand(COMMAND_COPY);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::MenuItem("Paste"_RU >> u8"Вставить"))
        {
            ExecCommand(COMMAND_PASTE);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::Separator();
        if (ImGui::MenuItem("Cut"_RU >> u8"Вырезать"))
        {
            ExecCommand(COMMAND_CUT);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::Separator();
        if (ImGui::MenuItem("Delete"_RU >> u8"Удалить"))
        {
            ExecCommand(COMMAND_DELETE_SELECTION);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::EndMenu();
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::BeginMenu("Visiblity"_RU >> u8"Видимость"))
    {
        if (ImGui::MenuItem("Hide Selected"_RU >> u8"Скрыть выделенное"))
        {
            ExecCommand(COMMAND_HIDE_SEL, FALSE);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::MenuItem("Hide Unselected"_RU >> u8"Скрыть невыделенное"))
        {
            ExecCommand(COMMAND_HIDE_UNSEL);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::MenuItem("Hide All"_RU >> u8"Скрыть ВСЁ"))
        {
            ExecCommand(COMMAND_HIDE_ALL, FALSE);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::Separator();
        if (ImGui::MenuItem("Unhide All"_RU >> u8"Показать ВСЁ"))
        {
            ExecCommand(COMMAND_HIDE_ALL, TRUE);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::EndMenu();
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    ImGui::Separator();
    // ------------------------------------------------------------------------------------------------------ //
    if (ImGui::MenuItem("Properties"_RU >> u8"Свойства"))
    {
        ExecCommand(COMMAND_SHOW_PROPERTIES);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

void UIMainForm::DrawRenderToolBar(ImVec2 Size)
{
    {
        ImGui::BeginGroup();
        m_tMenu->Load();
        {
            if (ImGui::BeginPopupContextItem("MenuScene"))
            {
                {
                    bool selected = psDeviceFlags.test(rsDrawSafeRect);
                    if (ImGui::MenuItem("Draw Safe Rect"_RU >> u8"Сжать вьюпорт", "", &selected))
                    {
                        psDeviceFlags.set(rsDrawSafeRect, selected);
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                {
                    bool selected = psDeviceFlags.test(rsDrawGrid);
                    if (ImGui::MenuItem("Draw Grid"_RU >> u8"Показать сетку", "", &selected))
                    {
                        psDeviceFlags.set(rsDrawGrid, selected);
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::Separator();
                {
                    bool selected = psDeviceFlags.test(rsFog);
                    if (ImGui::MenuItem("Fog"_RU >> u8"Туман", "", &selected))
                    {
                        psDeviceFlags.set(rsFog, selected);
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                {
                    if (ImGui::BeginMenu("Environment"_RU >> u8"Погода"))
                    {
                        bool selected = !psDeviceFlags.test(rsEnvironment);
                        if (ImGui::MenuItem("None", "", &selected))
                        {
                            psDeviceFlags.set(rsEnvironment, false);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        ImGui::Separator();
                        for (auto& i : g_pGamePersistent->Environment().WeatherCycles)
                        {
                            selected = psDeviceFlags.test(rsEnvironment) && i.first == g_pGamePersistent->Environment().CurrentCycleName;
                            if (ImGui::MenuItem(i.first.c_str(), "", &selected))
                            {
                                psDeviceFlags.set(rsEnvironment, true);
                                g_pGamePersistent->Environment().SetWeather(i.first.c_str(), true);
                                UI->RedrawScene();
                            }
                            if (ImGui::IsItemHovered())
                                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        }
                        ImGui::EndMenu();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::Separator();
                if (ImGui::BeginMenu("Render"_RU >> u8"Экран"))
                {
                    if (ImGui::BeginMenu("Quality"_RU >> u8"Качество"))
                    {
                        static bool selected[4] = {false, false, true, false};
                        if (ImGui::MenuItem("25%", "", &selected[0]))
                        {
                            selected[1] = selected[2] = selected[3] = false;
                            UI->SetRenderQuality(1 / 4.f);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::MenuItem("50%", "", &selected[1]))
                        {
                            selected[0] = selected[2] = selected[3] = false;
                            UI->SetRenderQuality(1 / 2.f);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::MenuItem("100%", "", &selected[2]))
                        {
                            selected[1] = selected[0] = selected[3] = false;
                            UI->SetRenderQuality(1.f);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::MenuItem("200%", "", &selected[3]))
                        {
                            selected[1] = selected[2] = selected[0] = false;
                            UI->SetRenderQuality(2.f);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        ImGui::EndMenu();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    if (ImGui::BeginMenu("Fill Mode"_RU >> u8"Режим отображения"))
                    {
                        bool selected[3] =
                        {
                            EDevice->dwFillMode == D3DFILL_POINT, EDevice->dwFillMode == D3DFILL_WIREFRAME,
                            EDevice->dwFillMode == D3DFILL_SOLID
                        };
                        if (ImGui::MenuItem("Point"_RU >> u8"Точки", "", &selected[0]))
                        {
                            EDevice->dwFillMode = D3DFILL_POINT;
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::MenuItem("Wireframe"_RU >> u8"Рёбра", "", &selected[1]))
                        {
                            EDevice->dwFillMode = D3DFILL_WIREFRAME;
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        if (ImGui::MenuItem("Solid"_RU >> u8"Норма", "", &selected[2]))
                        {
                            EDevice->dwFillMode = D3DFILL_SOLID;
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        ImGui::EndMenu();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    {
                        bool selected = psDeviceFlags.test(rsEdgedFaces);
                        if (ImGui::MenuItem("Edged Faces"_RU >> u8"Показать Рёбра", "", &selected))
                        {
                            psDeviceFlags.set(rsEdgedFaces, selected);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
                    {
                        bool selected = psDeviceFlags.test(rsLighting);
                        ;
                        if (ImGui::MenuItem("Lighting"_RU >> u8"Выключить Свет", "", &selected))
                        {
                            psDeviceFlags.set(rsLighting, selected);
                            UI->RedrawScene();
                        }
                        if (ImGui::IsItemHovered())
                            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::Separator();

                {
                    bool selected = psDeviceFlags.test(rsMuteSounds);
                    if (ImGui::MenuItem("Mute Sounds"_RU >> u8"Отключить звуки", "", &selected))
                    {
                        psDeviceFlags.set(rsMuteSounds, selected);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                {
                    bool selected = psDeviceFlags.test(rsRenderRealTime);
                    if (ImGui::MenuItem("Real Time"_RU >> u8"В реальном времени", "", &selected))
                    {
                        psDeviceFlags.set(rsRenderRealTime, selected);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::Separator();
                {
                    bool selected = psDeviceFlags.test(rsStatistic);
                    if (ImGui::MenuItem("Stats"_RU >> u8"Статистика", "", &selected))
                    {
                        psDeviceFlags.set(rsStatistic, selected);
                        UI->RedrawScene();
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
                ImGui::EndPopup();
            }
            if (ImGui::ImageButton(m_tMenu->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                ImGui::OpenPopup("MenuScene"_RU >> u8"MenuScene");
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Menu"_RU >> u8"Меню");
        }
        ImGui::EndGroup();
    }

    bool bIsSupportMove   = false;
    bool bIsSupportRotate = false;
    bool bIsSupportScale  = false;
    {
        ETAction Action = LTools->GetAction();

        if (Scene->GetTool(LTools->CurrentClassID()) && Scene->GetTool(LTools->CurrentClassID())->FindControl(0, etaSelect))
        {
            auto Control     = Scene->GetTool(LTools->CurrentClassID())->FindControl(0, etaSelect);
            bIsSupportMove   = Control->IsSupportMove();
            bIsSupportRotate = Control->IsSupportRotate();
            bIsSupportScale  = Control->IsSupportScale();
        }
        if (!bIsSupportMove && Action == etaSelect && LTools->GetGimzo()->GetType() == Gizmo::EType::Move)
        {
            LTools->GetGimzo()->SetType(Gizmo::EType::None);
        }
        if (!bIsSupportRotate && Action == etaSelect && LTools->GetGimzo()->GetType() == Gizmo::EType::Rotate)
        {
            LTools->GetGimzo()->SetType(Gizmo::EType::None);
        }
        if (!bIsSupportScale && Action == etaSelect && LTools->GetGimzo()->GetType() == Gizmo::EType::Scale)
        {
            LTools->GetGimzo()->SetType(Gizmo::EType::None);
        }

        ImGui::SameLine(0, ImGui::GetFontSize() * 5);
        ImGui::BeginGroup();

        m_tSelect->Load();
        {
            bool bPushColor = false;
            if (Action == etaSelect && LTools->GetGimzo()->GetType() == Gizmo::EType::None)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            if (ImGui::ImageButton(m_tSelect->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->SetAction(etaSelect);
                LTools->GetGimzo()->SetType(Gizmo::EType::None);
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Select"_RU >> u8"Выбрать");
        }

        ImGui::SameLine();

        {
            bool bPushColor = false;
            if (Action == etaAdd)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tAdd->Load();
            if (ImGui::ImageButton(m_tAdd->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->SetAction(etaAdd);
                LTools->GetGimzo()->SetType(Gizmo::EType::None);
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Add"_RU >> u8"Установить в сцене");
        }

        ImGui::SameLine();
        {
            bool bPushColor = false;

            ImGui::BeginDisabled(!bIsSupportMove);
            if (Action == etaSelect && LTools->GetGimzo()->GetType() == Gizmo::EType::Move)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tMove->Load();
            if (ImGui::ImageButton(m_tMove->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->SetAction(etaSelect);
                LTools->GetGimzo()->SetType(Gizmo::EType::Move);
            }

            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
            ImGui::EndDisabled();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Move"_RU >> u8"Двигать");
        }

        ImGui::SameLine();
        {
            bool bPushColor = false;
            ImGui::BeginDisabled(!bIsSupportScale);
            if (Action == etaSelect && LTools->GetGimzo()->GetType() == Gizmo::EType::Scale)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tScale->Load();
            if (ImGui::ImageButton(m_tScale->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->SetAction(etaSelect);
                LTools->GetGimzo()->SetType(Gizmo::EType::Scale);
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
            ImGui::EndDisabled();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Scale"_RU >> u8"Масштабирование");
        }

        ImGui::SameLine();
        {
            bool bPushColor = false;

            ImGui::BeginDisabled(!bIsSupportRotate);
            if (Action == etaSelect && LTools->GetGimzo()->GetType() == Gizmo::EType::Rotate)
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tRotate->Load();
            if (ImGui::ImageButton(m_tRotate->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->SetAction(etaSelect);
                LTools->GetGimzo()->SetType(Gizmo::EType::Rotate);
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
            ImGui::EndDisabled();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Rotate"_RU >> u8"Вращать");
        }

        ImGui::EndGroup();
    }
    {
        ImGui::SameLine(0, ImGui::GetFontSize() * 5);
        ImGui::BeginGroup();

        {
            bool bPushColor = false;
            if (Tools->GetSettings(etfNormalAlign))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tNSnap->Load();
            if (ImGui::ImageButton(
                    m_tNSnap->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
            {
                ExecCommand(COMMAND_SET_SETTINGS, etfNormalAlign, !Tools->GetSettings(etfNormalAlign));
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Normal Align"_RU >> u8"Привязка к нормалям");
        }
        ImGui::SameLine();

        m_tZoomSel->Load();
        if (ImGui::ImageButton(m_tZoomSel->surface_get(), ImVec2(16, ImGui::GetFontSize()), ImVec2(0, 0), ImVec2(0.5f, 1.f)))
        {
            ExecCommand(COMMAND_ZOOM_EXTENTS, TRUE);
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Focus on the selected object"_RU >> u8"Сфокусироваться на выбранном объекте");
        }
        ImGui::EndGroup();
    }
    {
        string_path Temp;
        ImGui::SameLine(0, ImGui::GetFontSize() * 5);
        ImGui::BeginGroup();

        {
            bool bPushColor = false;

            if (LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Move))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tGrid->Load();
            if (ImGui::ImageButton(m_tGrid->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->GetGimzo()->SwitchStep(Gizmo::EType::Move, !LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Move));
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Fixed object movement"_RU >> u8"Фиксированное перемещение объекта");
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 4);
        xr_sprintf(Temp, "%.2f", LTools->GetGimzo()->GetStep(Gizmo::EType::Move));
        if (ImGui::BeginCombo("##move", Temp, ImGuiComboFlags_None))
        {
            if (ImGui::Selectable("0.01", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 0.01f);
            }
            if (ImGui::Selectable("0.05", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 0.05f);
            }
            if (ImGui::Selectable("0.1", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 0.1f);
            }
            if (ImGui::Selectable("1", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 1.f);
            }
            if (ImGui::Selectable("2", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 2.f);
            }
            if (ImGui::Selectable("5", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 5.f);
            }
            if (ImGui::Selectable("50", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 50.f);
            }
            if (ImGui::Selectable("200", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 200.f);
            }
            if (ImGui::Selectable("1000", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Move, 1000.f);
            }
            ImGui::EndCombo();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("The choice of a fixed distance of movement of the object"_RU >> u8"Выбор фиксированного растояния перемещения объекта");
        }
        ImGui::SameLine(0, ImGui::GetFontSize());

        {
            bool bPushColor = false;
            if (LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Scale))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tScaleGrid->Load();
            if (ImGui::ImageButton(m_tScaleGrid->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->GetGimzo()->SwitchStep(Gizmo::EType::Scale, !LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Scale));
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Fixed Object Scaling"_RU >> u8"Фиксированное масштабирование объкта");
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 4);
        xr_sprintf(Temp, "%.2f", LTools->GetGimzo()->GetStep(Gizmo::EType::Scale));
        if (ImGui::BeginCombo("##scale", Temp, ImGuiComboFlags_None))
        {
            if (ImGui::Selectable("0.01", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 0.01f);
            }
            if (ImGui::Selectable("0.05", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 0.05f);
            }
            if (ImGui::Selectable("0.1", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 0.1f);
            }
            if (ImGui::Selectable("1", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 1.f);
            }
            if (ImGui::Selectable("2", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 2.f);
            }
            if (ImGui::Selectable("5", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 5.f);
            }
            if (ImGui::Selectable("50", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 50.f);
            }
            if (ImGui::Selectable("200", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 200.f);
            }
            if (ImGui::Selectable("1000", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Scale, 1000.f);
            }
            ImGui::EndCombo();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Setting a Fixed Object Scaling"_RU >> u8"Установка фиксированного масштабирования объкта");
        }
        ImGui::SameLine(0, ImGui::GetFontSize());

        {
            bool bPushColor = false;
            if (LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Rotate))
            {
                bPushColor = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Border));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));
            }
            m_tAngle->Load();
            if (ImGui::ImageButton(m_tAngle->surface_get(), ImVec2(16, ImGui::GetFontSize())))
            {
                LTools->GetGimzo()->SwitchStep(Gizmo::EType::Rotate, !LTools->GetGimzo()->IsStepEnable(Gizmo::EType::Rotate));
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Fixed object rotation angle"_RU >> u8"Фиксированный угол вращения объкта");
            }
            if (bPushColor)
            {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
        xr_sprintf(Temp, "%.5f", LTools->GetGimzo()->GetStep(Gizmo::EType::Rotate));
        if (ImGui::BeginCombo("##rotate", Temp, ImGuiComboFlags_None))
        {
            if (ImGui::Selectable("1.40625", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 1.40625f);
            }
            if (ImGui::Selectable("2.8125", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 2.8125f);
            }
            if (ImGui::Selectable("5.625", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 5.625f);
            }
            if (ImGui::Selectable("11.25", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 11.25f);
            }
            if (ImGui::Selectable("22.5", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 22.5f);
            }
            if (ImGui::Selectable("45", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 45.f);
            }
            if (ImGui::Selectable("90", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 90.f);
            }
            if (ImGui::Selectable("180", false))
            {
                LTools->GetGimzo()->SetStep(Gizmo::EType::Rotate, 180.f);
            }
            ImGui::EndCombo();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::SetTooltip("Set a fixed rotation angle of the object (in degrees)"_RU >> u8"Установка фиксированного угла вращения объкта(в градусах)");
        }
        ImGui::EndGroup();
    }
}
