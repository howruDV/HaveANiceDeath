#include "pch.h"
#include "CGameMgr.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CStateMachine.h>

#include <Scripts\CCamCtrlScript.h>

CGameObject* CGameMgr::m_pMainCamera = nullptr;

CGameMgr::CGameMgr()
	: CScript(GAMEMGR)
	, m_bTitleAcc(0.f)
{
	AddScriptParam(SCRIPT_PARAM::OBJECT, "GameEnding (Fail)", &m_GameEnding_Fail);
}

CGameMgr::~CGameMgr()
{
	GamePlayStatic::Stop2DBGM(L"sound\\field_time\\HAND World 8 WORLD V2 2.2.wav");
	GamePlayStatic::Stop2DSound(L"sound\\field_time\\Amb_Elt_W08_ClockMechanisms_Lp_01.wav");
}

void CGameMgr::begin()
{
	CPlayerMgr::GetPlayerScript()->StateMachine()->GetFSM()->ChangeState(L"Elevator_Out");

	GamePlayStatic::Play2DBGM(L"sound\\field_time\\HAND World 8 WORLD V2 2.2.wav", 0.3f);
	GamePlayStatic::Play2DSound(L"sound\\field_time\\Amb_Elt_W08_ClockMechanisms_Lp_01.wav", 0, 0.15f);

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pLevel)
	{
		int LayerIdx = pLevel->GetLayerIdxByName(L"Default");
		m_pMainCamera = pLevel->FindObjectByName(L"MainCamera", LayerIdx);
		m_pMainCamera->GetScriptByType<CCamCtrlScript>()->PushTransition(true);
	}
}

void CGameMgr::tick()
{
	CPlayerScript* pPlayerScript = CPlayerMgr::GetPlayerScript();

	if (pPlayerScript && pPlayerScript->IsDead() && !pPlayerScript->Animator2D()->IsPlaying())
	{
		GameEnding_Fail();
		m_bTitle = true;
	}

	if (m_bTitle)
	{
		m_bTitleAcc += DT;

		if (m_bTitleAcc > 5.f)
		{
			CLevel* pLevel = CLevel::LEVEL_LOAD(L"level\\Title.lv");
			CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::PLAY);
		}
	}
}

void CGameMgr::GameEnding_Fail()
{
	//m_pMainCamera->GetScriptByType<CCamCtrlScript>()->PushTransition(false);
}

void CGameMgr::GameEnding_Win()
{
}