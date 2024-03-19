#include "pch.h"
#include "UISound.h"

#include <Engine/CSound.h>

UISound::UISound()
	: UIAsset("Sound", "##Sound", ASSET_TYPE::SOUND)
{
}

UISound::~UISound()
{
}

void UISound::render_update()
{
    if (not GetAsset().Get())
        return;

    UIAsset::render_update();

    // 해당 텍스쳐 이미지 출력
    Ptr<CSound> pSound = (CSound*)GetAsset().Get();
    string strPath = string(pSound->GetRelativePath().begin(), pSound->GetRelativePath().end());
    strPath = path(strPath).filename().generic_string();

    ImGui::Text("Sound");
    ImGui::SameLine();
    ImGui::InputText("##SoundName", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);

    if (ImGui::Button("Play##SoundPlayBtn"))
    {
        if (m_RecentSound.Get())
            m_RecentSound->Stop();

        pSound->Play(1, 0.5f, true);
        m_RecentSound = pSound;
    }
}