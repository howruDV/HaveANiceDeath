#include "pch.h"
#include "CProgress_PlayerHP.h"
#include "CPlayerScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CProgress_PlayerHP::CProgress_PlayerHP()
    : CProgressBarScript(PROGRESS_PLAYERHP)
{
    m_fWidth = 392.f;
    m_fDecTime = 1.f;
    m_fIncTime = 1.f;
    m_fProgressSpeed = 8.f;
    m_DecColor = Vec4(1, 0, 0, 1);
    m_IncColor = Vec4(0, 1, 1, 1);
}

CProgress_PlayerHP::~CProgress_PlayerHP()
{
}

void CProgress_PlayerHP::begin()
{
    CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    if (pLevel)
    {
        int LayerIdx = pLevel->GetLayerIdxByName(L"Player");
        m_pUnit = pLevel->FindObjectByName(L"Death", LayerIdx)->GetScriptByType<CPlayerScript>();
    }

    CProgressBarScript::begin();
}

float CProgress_PlayerHP::GetProgress()
{
    return (float)m_pUnit->GetHPCur() / (float)m_pUnit->GetHPMax();
}
