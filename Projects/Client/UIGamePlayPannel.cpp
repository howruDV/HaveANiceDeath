#include "pch.h"
#include "UIGamePlayPannel.h"
#include "UIInspectorPannel.h"
#include "ObjectController.h"
#include "CLevelSaveLoad.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CTimeMgr.h>

UIGamePlayPannel::UIGamePlayPannel()
	: UI("GamePlay", "##GamePlayButtons")
    , m_BtnSize(ImVec2(20, 20))
    , m_ShowLayerWidth(300)
    , m_Inspector(nullptr)
{
    SetScroll(true);
}

UIGamePlayPannel::~UIGamePlayPannel()
{
}

void UIGamePlayPannel::begin()
{
    m_Inspector = (UIInspectorPannel*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	m_PlayIcon[0] = CAssetMgr::GetInst()->Load<CTexture>(L"texture//engine//iconPlay_white.png");
	m_PlayIcon[1] = CAssetMgr::GetInst()->Load<CTexture>(L"texture//engine//iconPlay_color.png");
	m_PauseIcon[0] = CAssetMgr::GetInst()->Load<CTexture>(L"texture//engine//iconPause_white.png");
	m_PauseIcon[1] = CAssetMgr::GetInst()->Load<CTexture>(L"texture//engine//iconPause_color.png");
	m_StopIcon = CAssetMgr::GetInst()->Load<CTexture>(L"texture//engine//iconStop_white.png");
}

void UIGamePlayPannel::render_update()
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    LEVEL_STATE State = pCurLevel->GetState();
    bool PlayEnable = false;
    bool PauseEnable = false;
    bool StopEnable = false;

    if (LEVEL_STATE::STOP == State || LEVEL_STATE::PAUSE == State)
        PlayEnable = true;
    if (LEVEL_STATE::PLAY == State)
        PauseEnable = true;
    if (LEVEL_STATE::PLAY == State || LEVEL_STATE::PAUSE == State)
        StopEnable = true;
   
    float windowWidth = ImGui::GetWindowSize().x;
    ImGui::SetCursorPosX(windowWidth * 0.5f - m_BtnSize.x * 3 - (m_ShowLayerWidth + 45));

    // layer
    ObjectController* pCtrler = m_Inspector->GetObjController();
    ImGui::PushItemWidth(m_ShowLayerWidth);
    if (pCtrler)
        pCtrler->render();
    ImGui::PopItemWidth();

    // play
    ImGui::SameLine();
    if (PlayEnable)
    {
        if (ImGui::ImageButton(m_PlayIcon[0]->GetSRV().Get(), m_BtnSize))
        {
            if (LEVEL_STATE::STOP == pCurLevel->GetState())
                CLevelSaveLoad::SaveLevel(pCurLevel, L"level//temp.lv");

            CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PLAY);
        }
    }
    else
        ImGui::ImageButton(m_PlayIcon[0]->GetSRV().Get(), m_BtnSize);

    // pause
    ImGui::SameLine();
    if (PauseEnable)
    {
        if (ImGui::ImageButton(m_PauseIcon[0]->GetSRV().Get(), m_BtnSize))
            CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PAUSE);
    }
    else
        ImGui::ImageButton(m_PauseIcon[0]->GetSRV().Get(), m_BtnSize);

    // stop
    ImGui::SameLine();
    if (StopEnable)
    {
        if (ImGui::ImageButton(m_StopIcon->GetSRV().Get(), m_BtnSize))
        {
            CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"level//temp.lv");
            CLevelMgr::GetInst()->ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

            // Inspector 의 타겟정보를 nullptr 로 되돌리기
            m_Inspector->SetTargetObject(nullptr);
        }
    }
    else
        ImGui::ImageButton(m_StopIcon->GetSRV().Get(), m_BtnSize);

    // FPS
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(230, 170, 0, 255));
    UINT strFPS = CTimeMgr::GetInst()->GetFPS();

    ImGui::Text("FPS: %d", strFPS);
    ImGui::PopStyleColor();
}