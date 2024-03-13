#include "stdafx.h"
#include "LogicEditor.h"

#include "CustomDataEditor.h"

UILogicEditor::UILogicEditor() {}

UILogicEditor& UILogicEditor::Instance()
{
    static UILogicEditor Editor;
    return               Editor;
}

void UILogicEditor::Draw()
{
    if (ImGui::BeginPopupContextItem("LogicEditor", 0))
    {
        ImGui::BeginGroup();

        if (ImGui::Button("Save"))
        {
            
        }
        ImGui::SameLine(0);
        if (ImGui::Button("SaveAll"))
        {
            
        }
        ImGui::SameLine(0);
        if (ImGui::Button("Reload"))
        {
            
        }
        ImGui::SameLine(0);
        if (ImGui::Button("Close"))
        {
            
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        size_t DatasNum = Datas.size();
        int    i        = 0;
        UICustomDataEditor* CurrentData = nullptr;
        for (auto it = Datas.begin(); it != Datas.end(); ++it)
        {
            if (ImGui::Button(it->first.c_str()))
            {
                CurrentData = &(it->second);
            }
            ++i;
            if (i < Datas.size()-1)
            {
                ImGui::SameLine();
            }
        }
        ImGui::EndGroup();
        if (!CurrentData)
        {
            CurrentData = &(Datas.begin()->second);
        }
        CurrentData->Draw();
        ImGui::EndPopup();
    }
}

void UILogicEditor::LoadCustomData(shared_str Key, LPCSTR CustomData)
{
    Datas.insert({Key, UICustomDataEditor(Key, CustomData)});
}
