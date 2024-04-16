#include "pch.h"
#include "CGameMgr.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CStateMachine.h>

CGameMgr::CGameMgr()
	: CScript(GAMEMGR)
{
	AddScriptParam(SCRIPT_PARAM::OBJECT, "GameEnding (Fail)", &m_GameEnding_Fail);
}

CGameMgr::~CGameMgr()
{
	GamePlayStatic::Stop2DBGM(L"sound\\field_time\\Amb_Lp_Level_W08_MysticDunes_01.wav");
}

void CGameMgr::begin()
{
	CPlayerMgr::GetPlayerScript()->StateMachine()->GetFSM()->ChangeState(L"Elevator_Out");

	GamePlayStatic::Play2DBGM(L"sound\\field_time\\Amb_Lp_Level_W08_MysticDunes_01.wav", 0.3f);
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
	CLevel* pLevel = CLevel::LEVEL_LOAD(L"level\\Ending_tmp.lv");
	CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::PLAY);

	/*m_GameEnding_Fail->Activate();

	CGameObject* pAnimObj = m_GameEnding_Fail->GetChildByName(L"Dead_Screen");

	if (!pAnimObj->Animator2D()->IsPlaying())
	pAnimObj->Animator2D()->Play(L"Dead_Screen", false);*/
}

void CGameMgr::GameEnding_Win()
{
}