#include "pch.h"
#include "CProgress_PlayerMP.h"
#include "CPlayerScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CProgress_PlayerMP::CProgress_PlayerMP()
    : CProgressBarScript(PROGRESS_PLAYERMP)
{
    m_fWidth = 333.f;
    m_fDecTime = 1.f;
    m_fIncTime = 1.f;
    m_fProgressSpeed = 8.f;
    m_DefaultColor = Vec4(0, 1, 1, 1);
}
CProgress_PlayerMP::~CProgress_PlayerMP()
{
}

void CProgress_PlayerMP::begin()
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

float CProgress_PlayerMP::GetProgress()
{
    return (float)m_Player->GetMPCur() / (float)m_Player->GetMPMax();
}