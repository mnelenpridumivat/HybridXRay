#include "stdafx.h"
#include "UIAboutForm.h"

XREPROPS_API extern bool bIsActorEditor;
ECORE_API extern bool    bIsLevelEditor;
ECORE_API extern bool    bIsParticleEditor = true;
ECORE_API extern bool    bIsShaderEditor   = true;

UIAboutForm*             UIAboutForm::Form = nullptr;

UIAboutForm::UIAboutForm()
{
    m_cocr_icon      = EDevice->Resources->_CreateTexture("ed\\bar\\icon_cocr");
    m_discord_icon   = EDevice->Resources->_CreateTexture("ed\\bar\\discord");
    m_github_icon    = EDevice->Resources->_CreateTexture("ed\\bar\\github");
    m_gitea_icon     = EDevice->Resources->_CreateTexture("ed\\bar\\gitea");
    m_vk_icon        = EDevice->Resources->_CreateTexture("ed\\bar\\vk");
    m_stalkerue_icon = EDevice->Resources->_CreateTexture("ed\\bar\\stalkerue");
}

UIAboutForm::~UIAboutForm() {}

void UIAboutForm::Draw()
{
    u32    buildIDLocal = Core.buildIDLocal;
    pcstr  buildName    = Core.buildName.c_str();
    pcstr  buildDate    = Core.buildDate;
    pcstr  commit       = Core.buildCommit;
    pcstr  branch       = Core.buildBranch;

    pcstr  versionSDK   = "SDK v.0.8.1b"_RU >> u8"СДК v.0.8.1b";

    pcstr  AEtitleCoP   = "X-Ray Actor Editor[CoP mode]"_RU >> u8"X-Ray Actor Editor[CoP режим]";
    pcstr  AEtitleCS    = "X-Ray Actor Editor[CS mode]"_RU >> u8"X-Ray Actor Editor[CS режим]";
    pcstr  AEtitleShoC  = "X-Ray Actor Editor[ShoC mode]"_RU >> u8"X-Ray Actor Editor[ShoC режим]";

    pcstr  LEtitleCoP   = "X-Ray Level Editor[CoP mode]"_RU >> u8"X-Ray Level Editor[CoP режим]";
    pcstr  LEtitleCS    = "X-Ray Level Editor[CS mode]"_RU >> u8"X-Ray Level Editor[CS режим]";
    pcstr  LEtitleShoC  = "X-Ray Level Editor[ShoC mode]"_RU >> u8"X-Ray Level Editor[ShoC режим]";

    pcstr  PEtitleCoP   = "X-Ray Particle Editor[CoP mode]"_RU >> u8"X-Ray Particle Editor[CoP режим]";
    pcstr  PEtitleCS    = "X-Ray Particle Editor[CS mode]"_RU >> u8"X-Ray Particle Editor[CS режим]";
    pcstr  PEtitleShoC  = "X-Ray Particle Editor[ShoC mode]"_RU >> u8"X-Ray Particle Editor[ShoC режим]";

    pcstr  SEtitleCoP   = "X-Ray Shader Editor[CoP mode]"_RU >> u8"X-Ray Shader Editor[CoP режим]";
    pcstr  SEtitleCS    = "X-Ray Shader Editor[CS mode]"_RU >> u8"X-Ray Shader Editor[CS режим]";
    pcstr  SEtitleShoC  = "X-Ray Shader Editor[ShoC mode]"_RU >> u8"X-Ray Shader Editor[ShoC режим]";

    ImVec2 sizeImage    = ImVec2(150.0f, 150.0f);   // Размер изображения, которое мы хотим сделать видимым
    ImVec2 sizeButton   = ImVec2(22.0f, 22.0f);     // Размер изображения, которое мы хотим сделать видимым

    ImGui::BeginChild("Center", ImVec2(400, 500), true);
    // Картинка
    {
        m_cocr_icon->Load();
        ImGui::SameLine(125, 0);
        ImGui::Image(m_cocr_icon->surface_get(), ImVec2(sizeImage));
        // ImGui::Separator();
        ImGui::NewLine();
        ImGui::SameLine(135, 0);
        ImGui::TextColored(ImVec4(0, 1, 0, 0.9), "CoCRTeam[HybridXRay]"_RU >> u8"CoCRTeam[HybridXRay]");
    }
    ImGui::Separator();
    ImGui::NewLine();
    ImGui::Spacing();
    // Версия Редактора
    {
        ImGui::SameLine(0, 15);
        ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Version SDK:     [ %s ]"_RU >> u8"Версия СДК:     [ %s ]", versionSDK);
        ImGui::NewLine();
        ImGui::SameLine(0, 15);
        if (bIsActorEditor)
        {
            if (xrGameManager::GetGame() == EGame::COP)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                  %s ", AEtitleCoP);
            else if (xrGameManager::GetGame() == EGame::CS)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                  %s ", AEtitleCS);
            else if (xrGameManager::GetGame() == EGame::SHOC)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                  %s ", AEtitleShoC);
        }
        else if (bIsLevelEditor)
        {
            if (xrGameManager::GetGame() == EGame::COP)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                 [ %s ]", LEtitleCoP);
            else if (xrGameManager::GetGame() == EGame::CS)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                 [ %s ]", LEtitleCS);
            else if (xrGameManager::GetGame() == EGame::SHOC)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                 [ %s ]", LEtitleShoC);
        }
        else if (bIsParticleEditor)
        {
            if (xrGameManager::GetGame() == EGame::COP)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                 [ %s ]", PEtitleCoP);
            else if (xrGameManager::GetGame() == EGame::CS)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                 [ %s ]", PEtitleCS);
            else if (xrGameManager::GetGame() == EGame::SHOC)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                 [ %s ]", PEtitleShoC);
        }
        else if (bIsShaderEditor)
        {
            if (xrGameManager::GetGame() == EGame::COP)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                 [ %s ]", SEtitleCoP);
            else if (xrGameManager::GetGame() == EGame::CS)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                 [ %s ]", SEtitleCS);
            else if (xrGameManager::GetGame() == EGame::SHOC)
                ImGui::TextColored(ImVec4(1, 1, 0, 0.8), "Editor:                 [ %s ]", SEtitleShoC);
        }
    }
    ImGui::NewLine();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::NewLine();
    // Инфо Build
    {
        ImGui::SameLine(135, 0);
        ImGui::TextColored(ImVec4(1, 0, 0, 0.9), "Information about Build:"_RU >> u8"Информация о Билде:");
        ImGui::NewLine();
        ImGui::Spacing();
        ImGui::SameLine(0, 15);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Build:        [ %s ]"_RU >> u8"Билд:        [ %s ]", buildName);
        ImGui::NewLine();
        ImGui::SameLine(0, 15);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Data:         [ %s ]"_RU >> u8"Дата:        [ %s ]", buildDate);
        ImGui::NewLine();
        ImGui::SameLine(0, 15);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Commit:  [ %s ]"_RU >> u8"Коммит:  [ %s ]", commit);
        ImGui::NewLine();
        ImGui::SameLine(0, 15);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Branch:    [ %s ]"_RU >> u8"Ветка:      [ %s ]", branch);
    }
    ImGui::NewLine();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::NewLine();
    ImGui::Spacing();
    // Копирайты
    {
        ImGui::SameLine(0, 15);
        ImGui::TextColored(ImVec4(0, 1, 1, 0.7), "(C) GSC Game World 2003-2009");
        ImGui::NewLine();
        // Red Projects
        ImGui::SameLine(0, 15);
        ImGui::TextColored(ImVec4(0, 1, 1, 0.7), "(C) Red Projects 2020-2022");
        // Кнопка в Discord
        {
            ImGui::SameLine(0, 40);
            m_discord_icon->Load();
            if (ImGui::ImageButton(m_discord_icon->surface_get(), sizeButton))
            {
                // Открыть ссылку
                std::system("start https://discord.gg/dbBdK2GxKf");
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Open the Discord server 'Red Projects'"_RU >> u8"Открыть сервер Discord 'Red Projects'");
            }
        }
        // Кнопка на GitHub
        {
            ImGui::SameLine(0, 5);
            m_github_icon->Load();
            if (ImGui::ImageButton(m_github_icon->surface_get(), sizeButton))
            {
                // Открыть ссылку
                std::system("start https://github.com/RedPandaProjects/XRayEngine");
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Open the 'Red Projects' repository on GitHub"_RU >> u8"Открыть репозиторий 'Red Projects' на GitHub");
            }
        }
        // Кнопка на Gitea
        {
            ImGui::SameLine(0, 5);
            m_gitea_icon->Load();
            if (ImGui::ImageButton(m_gitea_icon->surface_get(), sizeButton))
            {
                // Открыть ссылку
                std::system("start https://git.s2ue.org/RedProjects/XRayEngine");
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Open the 'Red Projects' repository on Gitea"_RU >> u8"Открыть репозиторий 'Red Projects' на Gitea");
            }
        }
        // Кнопка на сайт
        {
            ImGui::SameLine(0, 5);
            m_stalkerue_icon->Load();
            if (ImGui::ImageButton(m_stalkerue_icon->surface_get(), ImVec2(88, 22)))
            {
                // Открыть ссылку
                std::system("start https://s2ue.org");
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Open the 'Red Projects' website"_RU >> u8"Открыть сайт 'Red Projects'");
            }
        }
        ImGui::NewLine();
        // CoCRTeam[HybridXRay]
        ImGui::SameLine(0, 15);
        ImGui::TextColored(ImVec4(0, 1, 1, 0.7), "(C) CoCRTeam[HybridXRay] 2023");
        // Кнопка в ВК
        {
            ImGui::SameLine(0, 8);
            m_vk_icon->Load();
            if (ImGui::ImageButton(m_vk_icon->surface_get(), sizeButton))
            {
                // Открыть ссылку
                std::system("start https://vk.com/cocr_team");
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Open the VK group of the 'CoCRTeam' team, the 'HybridXRay' project"_RU >> u8"Открыть группу ВК команды 'CoCRTeam', проекта 'HybridXRay'");
            }
        }
        // Кнопка на GitHub
        {
            ImGui::SameLine(0, 5);
            m_github_icon->Load();
            if (ImGui::ImageButton(m_github_icon->surface_get(), sizeButton))
            {
                // Открыть ссылку
                std::system("start https://github.com/Roman-n/HybridXRay?tab=readme-ov-file#readme");
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                ImGui::SetTooltip("Open the 'HybridXRay' repository on GitHub"_RU >> u8"Открыть репозиторий 'HybridXRay' на GitHub");
            }
        }
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::SameLine(182, 0);
    if (ImGui::Button("Close"_RU >> u8"Закрыть"))
    {
        bOpen = false;
        ImGui::CloseCurrentPopup();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

void UIAboutForm::Update()
{
    if (Form)
    {
        if (!Form->IsClosed())
        {
            if (ImGui::BeginPopupModal("About Edtor"_RU >> u8"О Редакторе", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize, true))
            {
                Form->Draw();
                ImGui::EndPopup();
            }
        }
        else
        {
            xr_delete(Form);
        }
    }
}

void UIAboutForm::Show()
{
    VERIFY(!Form);
    Form = xr_new<UIAboutForm>();
}
