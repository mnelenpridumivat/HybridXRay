#include "stdafx.h"
#include "UIMinimapEditorForm.h"
UIMinimapEditorForm*    UIMinimapEditorForm::Form = nullptr;
bool                    UIMinimapEditorForm::bOpen = false;
UIMinimapEditorForm::UIMinimapEditorForm()
{
    m_TextureRemove = nullptr;
    m_Texture = nullptr;
}

UIMinimapEditorForm::~UIMinimapEditorForm()
{
    if (m_Texture)m_Texture->Release();
}

void UIMinimapEditorForm::Draw()
{
    if (m_TextureRemove)
    {
        m_TextureRemove->Release();
        m_TextureRemove = nullptr;
    }
    if (m_Texture == nullptr)
    {
        u32 mem = 0;
        m_Texture = RImplementation.texture_load("ed\\ed_nodata", mem);
    }
    ImGui::Image(m_Texture, ImVec2(330, 530));
    if (ImGui::Button("Load"))LoadClick();
 

}

void UIMinimapEditorForm::Update()
{
    if (Form)
    {
        if (!Form->IsClosed())
        {
            if (ImGui::BeginPopupModal("MinimapEditor", &bOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize, true))
            {
                Form->Draw();
                ImGui::EndPopup();
            }
            else
            {
                Form->bOpen = false;
                ImGui::CloseCurrentPopup();
            }
        }
        else
        {
            xr_delete(Form);
        }

    }
}

void UIMinimapEditorForm::Show()
{
	VERIFY(!Form);
	Form = xr_new< UIMinimapEditorForm>();
    bOpen = true;
}

void UIMinimapEditorForm::LoadClick()
{
}
