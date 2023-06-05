#include "stdafx.h"

UIWorldPropertiesFrom::UIWorldPropertiesFrom() {}

UIWorldPropertiesFrom::~UIWorldPropertiesFrom() {}

void UIWorldPropertiesFrom::Draw()
{
    if (bOpen)
    {
        if (ImGui::Begin("World Properties"_RU >> u8"Мировые свойства", &bOpen))
        {
            LTools->GetWorldProperties()->Draw();
        }
        ImGui::End();
    }
}
