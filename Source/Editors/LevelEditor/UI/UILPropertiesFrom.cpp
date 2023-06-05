#include "stdafx.h"

UILPropertiesFrom::UILPropertiesFrom() {}

UILPropertiesFrom::~UILPropertiesFrom() {}

void UILPropertiesFrom::Draw()
{
    if (bOpen)
    {
        if (ImGui::Begin("Properties"_RU >> u8"Свойства", &bOpen))
        {
            LTools->GetProperties()->Draw();
        }
        ImGui::End();
    }
}
