#include "pch.h"
#include "CGameMgr.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CGameMgr::CGameMgr()
	: CScript(GAMEMGR)
{
	AddScriptParam(SCRIPT_PARAM::OBJECT, "GameEnding (Fail)", &m_GameEnding_Fail);
}

CGameMgr::~CGameMgr()
{
}

void CGameMgr::tick()
{
	CPlayerScript* pPlayerScript = CPlayerMgr::GetPlayerScript();

	if (pPlayerScript && pPlayerScript->IsDead() && !pPlayerScript->Animator2D()->IsPlaying())
	{
		GameEnding_Fail();
	}
}

void CGameMgr::GameEnding_Fail()
{
	// @TODO : Level 새로 파는게 가장 좋음
	m_GameEnding_Fail->Activate();

	CGameObject* pAnimObj = m_GameEnding_Fail->GetChildByName(L"Dead_Screen");

	if (!pAnimObj->Animator2D()->IsPlaying())
	pAnimObj->Animator2D()->Play(L"Dead_Screen", false);
}

void CGameMgr::GameEnding_Win()
{
}