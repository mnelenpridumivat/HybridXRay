#include "stdafx.h"
#include "imgui_plot.h"

UIPropertiesItem::UIPropertiesItem(shared_str _Name, UIPropertiesForm* _PropertiesFrom, SLocalizedString _HintText): UITreeItem(_Name, _HintText), PropertiesFrom(_PropertiesFrom)
{
    PItem = nullptr;
}

UIPropertiesItem::~UIPropertiesItem() {}

void UIPropertiesItem::Draw()
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    if (PItem && PItem->m_Flags.test(PropItem::flShowCB))
    {
        if (ImGui::CheckboxFlags("##value", &PItem->m_Flags.flags, PropItem::flCBChecked))
        {
            PItem->OnChange();
            PropertiesFrom->Modified();
        }
        ImGui::SameLine(0, 2);
    }
    EPropType type = EPropType::PROP_UNDEF;
    if (PItem)
    {
        type = PItem->Type();
    }
    if (Items.size() || type == PROP_WAVE)
    {
        ImGuiTreeNodeFlags FolderFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
        bool               open        = ImGui::TreeNodeEx(Name.c_str(), FolderFlags);

        ImGui::TableNextColumn();
        DrawItem();
        if (open)
        {
            for (UITreeItem* Item: Items)
            {
                static_cast<UIPropertiesItem*>(Item)->Draw();
            }
            if (type == PROP_WAVE)
            {
                WaveValue* V = dynamic_cast<WaveValue*>(PItem->GetFrontValue());

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                //ImGui::SameLine(0, 2);
                ImGui::TreeNodeEx("Function:", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                //ImGui::Text("Function:");
                ImGui::TableNextColumn();
                const char* const Items[]         = {"Constant", "Sin", "Triangle", "Sqare", "Sawtooth", "InvSawTooth"};
                int               CurrentFunction = V->value->F;
                ImGui::Combo("Combo", &CurrentFunction, Items, 6);
                V->value->F = static_cast<WaveForm::EFunction>(CurrentFunction);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                //ImGui::SameLine(0, 2);
                ImGui::TreeNodeEx("Arg1:", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                //ImGui::Text("Arg1:");
                ImGui::TableNextColumn();
                ImGui::InputFloat("Arg1", &(V->value->arg[0]), 0.01, 1, 6);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                //ImGui::SameLine(0, 2);
                ImGui::TreeNodeEx("Arg2:", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                //ImGui::Text("Arg2:");
                ImGui::TableNextColumn();
                ImGui::InputFloat("Arg2", &(V->value->arg[1]), 0.01, 1, 6);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                //ImGui::SameLine(0, 2);
                ImGui::TreeNodeEx("Arg3:", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                //ImGui::Text("Arg3:");
                ImGui::TableNextColumn();
                ImGui::InputFloat("Arg3", &(V->value->arg[2]), 0.01, 1, 6);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                //ImGui::SameLine(0, 2);
                ImGui::TreeNodeEx("Arg4:", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                //ImGui::Text("Arg4:");
                ImGui::TableNextColumn();
                ImGui::InputFloat("Arg4", &(V->value->arg[3]), 0.01, 1, 6);
            }
            ImGui::TreePop();
        }
    }
    else
    {
        ImGui::TreeNodeEx(Name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
        ImGui::TableNextColumn();
        DrawItem();
    }
}

void UIPropertiesItem::DrawRoot()
{
    VERIFY(PItem == nullptr);
    for (UITreeItem* Item: Items)
    {
        static_cast<UIPropertiesItem*>(Item)->Draw();
    }
}

void UIPropertiesItem::DrawItem()
{
    if (!PItem)
        return;

    EPropType type = PItem->Type();
    //
    ImGui::BeginGroup();
    switch (type)
    {
        case PROP_CANVAS:
        {
            if (PItem->m_Flags.test(PropItem::flMixed))
            {
                ImGui::TextDisabled(PItem->GetDrawText().c_str());
            }
            else
            {
                ImGui::PushItemWidth(-1);
                CanvasValue* val = dynamic_cast<CanvasValue*>(PItem->GetFrontValue());
                R_ASSERT(val);
                if (!val->OnDrawCanvasEvent.empty())
                    val->OnDrawCanvasEvent(val);
            }
        }
        break;
        case PROP_BUTTON:

            if (PItem->m_Flags.test(PropItem::flMixed))
            {
                ImGui::TextDisabled(PItem->GetDrawText().c_str());
            }
            else
            {
                ImGui::PushID(Name.c_str());
                bool         bRes  = false;
                bool         bSafe = false;
                ButtonValue* V     = dynamic_cast<ButtonValue*>(PItem->GetFrontValue());
                R_ASSERT(V);
                if (!V->value.empty())
                {
                    ImGui::PushItemWidth(-1);
                    float size   = float(ImGui::CalcItemWidth());
                    float dx     = floorf(size / float(V->value.size()));
                    float offset = size - (dx * V->value.size());
                    V->btn_num   = V->value.size();
                    for (RStringVecIt it = V->value.begin(); it != V->value.end(); it++)
                    {
                        int k = it - V->value.begin();
                        if (ImGui::Button(it->c_str(), ImVec2(dx + offset, 0)))
                        {
                            V->btn_num = k;

                            bRes |= V->OnBtnClick(bSafe);
                        }
                        offset = 0;
                        ImGui::SameLine(0, 2);
                    }
                }
                else
                {
                    ImGui::Text("");
                }
                ImGui::PopID();
            }
            break;
        case PROP_WAVE:
        {
            if (PItem->m_Flags.test(PropItem::flMixed))
            {
                ImGui::TextDisabled(PItem->GetDrawText().c_str());
            }
            else
            {
                bool         bRes  = false;
                bool         bSafe = false;
                WaveValue*   V     = dynamic_cast<WaveValue*>(PItem->GetFrontValue());
                if (ImGui::Button("Show plot"))
                {
                    ImGui::OpenPopup("Wave Editor");
                }
                if (ImGui::BeginPopupModal("Wave Editor", nullptr , ImGuiWindowFlags_NoResize))
                {
                    //ImGui::PlotLines;
                    constexpr size_t PlotPointsNum = 128;

                    ImGui::SetWindowSize(ImVec2(PlotPointsNum*2 + 300, PlotPointsNum + 52));
                    ImGui::BeginTable("Wave Form", 2, 0, ImVec2(PlotPointsNum*2+256, PlotPointsNum));
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        ImGui::BeginTable("Wave params", 2, 0, ImVec2(256, PlotPointsNum));
                        {
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("Function:");
                            ImGui::TableNextColumn();
                            const char* const Items[]         = {"Constant", "Sin", "Triangle", "Sqare", "Sawtooth", "InvSawTooth"};
                            int              CurrentFunction = V->value->F;
                            ImGui::Combo("", &CurrentFunction, Items, 6);
                            V->value->F = static_cast<WaveForm::EFunction>(CurrentFunction);

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("Arg1:");
                            ImGui::TableNextColumn();
                            ImGui::InputFloat("Arg1", &(V->value->arg[0]), 0.01, 1, 6);

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("Arg2:");
                            ImGui::TableNextColumn();
                            ImGui::InputFloat("Arg2", &(V->value->arg[1]), 0.01, 1, 6);

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("Arg3:");
                            ImGui::TableNextColumn();
                            ImGui::InputFloat("Arg3", &(V->value->arg[2]), 0.01, 1, 6);

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("Arg4:");
                            ImGui::TableNextColumn();
                            ImGui::InputFloat("Arg4", &(V->value->arg[3]), 0.01, 1, 6);

                            ImGui::TableSetColumnWidth(0, 64);
                            ImGui::TableSetColumnWidth(1, 192);

                        }
                        ImGui::EndTable();
                        ImGui::TableNextColumn();

                        static float xValues[PlotPointsNum];
                        static float yValues[PlotPointsNum];
                        for (int i = 0; i < PlotPointsNum; ++i)
                        {
                            constexpr float div = 10.0f / PlotPointsNum;
                            xValues[i]          = div * i;
                            yValues[i] = V->value->arg[0] + V->value->arg[1] * V->value->Func((xValues[i] + V->value->arg[2]) * V->value->arg[3]);
                        }
                        ImGui::PlotConfig conf;
                        conf.values.xs    = xValues;
                        conf.values.ys    = yValues;
                        conf.values.count = PlotPointsNum;
                        conf.grid_x.show  = true;
                        conf.grid_y.show  = true;
                        conf.frame_size   = ImVec2(PlotPointsNum*2, PlotPointsNum);
                        ImGui::Plot("y = arg1 + arg2*func((time + arg3)*arg4)", conf);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        if (ImGui::Button("Close"))
                        {
                            ImGui::CloseCurrentPopup();
                        }

                    }

                    ImGui::EndTable();
                    ImGui::EndPopup();
                }
                
            }
        }
        case PROP_UNDEF:
        {
            break;
        }
        case PROP_CAPTION:
        {
            ImGui::TextDisabled(PItem->GetDrawText().c_str());
        }
        break;

        default:
            ImGui::PushID(Name.c_str());
            ShowHintIfHovered();

            if (PropertiesFrom->IsReadOnly())
            {
                if (type == PROP_BOOLEAN)
                {
                    FlagValueCustom* V = dynamic_cast<FlagValueCustom*>(PItem->GetFrontValue());
                    VERIFY(V);
                    ImGui::TextDisabled(V->GetValueEx() ? "true" : "false");
                }
                else
                {
                    ImGui::TextDisabled(PItem->GetDrawText().c_str());
                }
            }
            else if (PItem->m_Flags.test(PropItem::flMixed) && !PItem->m_Flags.test(PropItem::flIgnoreMixed))
            {
                if (ImGui::Button("(Mixed)", ImVec2(-1, 0)))
                {
                    RemoveMixed();
                }
            }
            else
            {
                if (PItem->m_Flags.test(PropItem::flDisabled))
                {
                    if (type == PROP_FLAG)
                    {
                        FlagValueCustom* V = dynamic_cast<FlagValueCustom*>(PItem->GetFrontValue());
                        VERIFY(V);
                        ImGui::TextDisabled(V->GetValueEx() ? "true" : "false");
                    }
                    else
                    {
                        ImGui::TextDisabled(PItem->GetDrawText().c_str());
                    }
                }
                else
                {
                    ImGui::PushItemWidth(-1);
                    DrawProp();
                }
            }
            ImGui::PopID();
            break;
    }
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

UITreeItem* UIPropertiesItem::CreateItem(shared_str _Name, SLocalizedString _HintText)
{
    return xr_new<UIPropertiesItem>(_Name, PropertiesFrom, _HintText);
}
