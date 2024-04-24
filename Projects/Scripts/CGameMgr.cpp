#include "pch.h"
#include "CGameMgr.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"
#include "CCamCtrlScript.h"

#include <Engine\CGameObject.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CTransform.h>

CGameObject* CGameMgr::m_pMainCamera = nullptr;

CGameMgr::CGameMgr()
	: CScript(GAMEMGR)
	, m_pCurLevelMgr(nullptr)
	, m_vHUDPosFromScreenLT(Vec2(120.f, 80.f))
{
}

CGameMgr::~CGameMgr()
{
	if (m_pCurLevelMgr)
		delete m_pCurLevelMgr;
}

void CGameMgr::begin()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (pLevel)
	{
		// camera
		m_pMainCamera = pLevel->FindObjectByName(L"MainCamera", 0);
		m_pMainCamera->GetScriptByType<CCamCtrlScript>()->PushTransition(false);

		// level mgr
		CreateLevelMgr(pLevel);
		if (m_pCurLevelMgr)
			m_pCurLevelMgr->begin();
	}
}

void CGameMgr::tick()
{
	CPlayerScript* pPlayerScript = CPlayerMgr::GetPlayerScript();

	if (pPlayerScript && pPlayerScript->IsDead() && !pPlayerScript->Animator2D()->IsPlaying())
	{
		GameEnding_Fail();
		//m_bTitle = true;
	}

	//if (m_bTitle)
	//{
	//	m_bTitleAcc += DT;

	//	if (m_bTitleAcc > 5.f)
	//	{
	//		CLevel* pLevel = CLevel::LEVEL_LOAD(L"level\\Title.lv");
	//		CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::PLAY);
	//	}
	//}

	if (m_pCurLevelMgr)
		m_pCurLevelMgr->tick();
}

void CGameMgr::RescaleHUD(CGameObject* HUD)
{
	if (!HUD)
		return;

	// pos
	Vec3 Pos = HUD->Transform()->GetRelativePos();
	Pos.x = -CRenderMgr::GetInst()->GetWinResol().x / 2.f + m_vHUDPosFromScreenLT.x * CRenderMgr::GetInst()->GetWinScale().x;
	Pos.y = CRenderMgr::GetInst()->GetWinResol().y / 2.f - m_vHUDPosFromScreenLT.y * CRenderMgr::GetInst()->GetWinScale().y;
	HUD->Transform()->SetRelativePos(Pos);

	list<CGameObject*> vecChild{ HUD };

	while (!vecChild.empty())
	{
		CGameObject* Cur = vecChild.front();
		vecChild.pop_front();

		// pos
		if (Cur->GetParent())
		{
			Pos = Cur->Transform()->GetRelativePos();
			Pos.x *= CRenderMgr::GetInst()->GetWinScale().x;
			Pos.y *= CRenderMgr::GetInst()->GetWinScale().y;
			Cur->Transform()->SetRelativePos(Pos);
		}

		// scale
		if (Cur->Transform()->IsAbsolute())
		{
			Vec3 Scale = Cur->Transform()->GetRelativeScale();
			Scale.x *= CRenderMgr::GetInst()->GetWinScale().x;
			Scale.y *= CRenderMgr::GetInst()->GetWinScale().y;
			Cur->Transform()->SetRelativeScale(Scale);
		}

		// get child
		vector<CGameObject*> vec = Cur->GetChild();
		for (CGameObject* it : vec)
			vecChild.push_back(it);
	}
}

void CGameMgr::GameEnding_Fail()
{
	m_pMainCamera->GetScriptByType<CCamCtrlScript>()->PushTransition(true);
}

void CGameMgr::GameEnding_Win()
{
}


// ======================================== [Func] Create Level Mgr ========================================
#include "CLevelMgrScript_W09_Field1.h";

void CGameMgr::CreateLevelMgr(CLevel* _Level)
{
	wstring strLevel = _Level->GetName();

	if (strLevel == "G0_Title.lv")
		m_pCurLevelMgr = nullptr;
	else if (strLevel == L"G1_Ending_Fail.lv")
		m_pCurLevelMgr = nullptr;
	else if (strLevel == L"Field1" || strLevel == L"WorkSpace.lv" || strLevel == L"W09_Field1.lv")
	{
		CLevelMgrScript_W09_Field1* pScript = new CLevelMgrScript_W09_Field1;
		pScript->SetGameMgr(this);
		m_pCurLevelMgr = pScript;
		m_pMainCamera->GetScriptByType<CCamCtrlScript>()->SetCheckCamArea(&CLevelMgrScript_W09_Field1::CheckCamArea);
	}
	else if (strLevel == L"W09_MiniBoss_Syster")
		m_pCurLevelMgr = nullptr;
	else if (strLevel == L"W09_Boss_Time")
		m_pCurLevelMgr = nullptr;
	else if (strLevel == L"W10_Boss_Life")
		m_pCurLevelMgr = nullptr;
}