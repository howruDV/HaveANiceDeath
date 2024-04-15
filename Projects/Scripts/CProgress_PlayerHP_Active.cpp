#include "pch.h"
#include "CProgress_PlayerHP_Active.h"
#include "CPlayerScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CProgress_PlayerHP_Active::CProgress_PlayerHP_Active()
    : CProgressBarScript(PROGRESS_PLAYERHP_ACTIVE)
    , m_Player(nullptr)
{
    m_fWidth = 392.f;
    m_DefaultColor = Vec4(0.6f, 0.6f, 0.6f, 1.f);
}

CProgress_PlayerHP_Active::~CProgress_PlayerHP_Active()
{
}

void CProgress_PlayerHP_Active::begin()
{
    CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    if (pLevel)
    {
        int LayerIdx = pLevel->GetLayerIdxByName(L"Player");
        m_Player = pLevel->FindObjectByName(L"Death", LayerIdx)->GetScriptByType<CPlayerScript>();
        m_pUnit = m_Player;
    }

    CProgressBarScript::begin();
}

float CProgress_PlayerHP_Active::GetProgress()
{
    return (float)m_Player->GetHPActive() / (float)m_Player->GetHPMax();
}