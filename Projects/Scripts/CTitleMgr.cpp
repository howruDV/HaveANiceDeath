#include "pch.h"
#include "CTitleMgr.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CTransform.h>

CTitleMgr::CTitleMgr()
	: CScript(TITLEMGR)
	, m_GameStart(false)
{
}

CTitleMgr::~CTitleMgr()
{
	GamePlayStatic::Stop2DBGM(L"sound\\title\\Music_HAND_MenuTheme_230223.wav");
}

void CTitleMgr::begin()
{
	GamePlayStatic::Play2DBGM(L"sound\\title\\Music_HAND_MenuTheme_230223.wav", 0.4f);

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pLevel)
	{
		int LayerIdx = pLevel->GetLayerIdxByName(L"Background");
		CGameObject* pBackground = pLevel->FindObjectByName(L"Background", LayerIdx);
		pBackground->Animator2D()->Play(L"Title", true);
		pBackground->Transform()->SetRelativeScale(Vec3(1600,900,9));

		m_Transition = pLevel->FindObjectByName(L"Transition", 0);
		m_Transition->Deactivate();
	}
}

void CTitleMgr::tick()
{
	if (m_GameStart && !m_Transition->Animator2D()->IsPlaying())
	{
		CLevel* pLevel = CLevel::LEVEL_LOAD(L"level\\Field1");
		CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::PLAY);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		GamePlayStatic::Play2DSound(L"sound\\title\\Menu_Main_But_Play_01.wav", 1, 0.3f);
		m_Transition->Activate();
		m_Transition->Animator2D()->Play(L"Transition");
		m_Transition->Transform()->SetRelativeScale(Vec3(1600, 900, 0));
		m_GameStart = true;
	}
}