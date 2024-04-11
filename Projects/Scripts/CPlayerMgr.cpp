#include "pch.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CGameObject* CPlayerMgr::m_pPlayer = nullptr;
CPlayerScript* CPlayerMgr::m_pPlayerScript = nullptr;

CPlayerMgr::CPlayerMgr()
	: CScript(PLAYERMGR)
{
}

CPlayerMgr::~CPlayerMgr()
{
}

void CPlayerMgr::begin()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pLevel)
	{
		int LayerIdx = pLevel->GetLayerIdxByName(L"Player");
		CGameObject* pObj = pLevel->FindObjectByName(L"Death", LayerIdx);
		SetPlayer(pObj);
	}
}

void CPlayerMgr::tick()
{
	if (!m_pPlayer)
	{
		m_pPlayerScript = nullptr;
		MessageBoxA(nullptr, "Can't Find Player!", "Failed to Find Player", MB_OK);
	}
}

void CPlayerMgr::SetPlayer(CGameObject* _obj)
{
	//if (m_pPlayer)
	//{
	//	FUnitInfo& pCurInfo = _obj->GetScript<CUnitScript>()->GetUnitInfo();
	//	pCurInfo.m_fMaxHP = m_pPlayerInfo.m_fMaxHP;
	//	pCurInfo.m_fHP = m_pPlayerInfo.m_fHP;
	//}

	m_pPlayer = _obj;
	m_pPlayerScript = m_pPlayer->GetScriptByType<CPlayerScript>();

	if (not m_pPlayerScript)
	{
		MessageBoxA(nullptr, "Can't Find Player Script From Player!", "Failed to Find PlayerScript", MB_OK);
		assert(nullptr);
	}
}