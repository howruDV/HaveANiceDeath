#include "pch.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <Scripts\CInvenMgr.h>

CGameObject* CPlayerMgr::m_pPlayer = nullptr;
CPlayerScript* CPlayerMgr::m_pPlayerScript = nullptr;
CInvenMgr* CPlayerMgr::m_pInvenScript = nullptr;

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

		LayerIdx = pLevel->GetLayerIdxByName(L"Manager");
		pObj = pLevel->FindObjectByName(L"Manager", LayerIdx);
		if (pObj)
			m_pInvenScript = pObj->GetScriptByType<CInvenMgr>();
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
	m_pPlayer = _obj;
	m_pPlayerScript = m_pPlayer->GetScriptByType<CPlayerScript>();

	if (not m_pPlayerScript)
	{
		MessageBoxA(nullptr, "Can't Find Player Script From Player!", "Failed to Find PlayerScript", MB_OK);
		assert(nullptr);
	}
}