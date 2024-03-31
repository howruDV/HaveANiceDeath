#include "pch.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

#include <Engine/CGameObject.h>

CPlayerMgr* CPlayerMgr::m_This = nullptr;

CPlayerMgr::CPlayerMgr()
	: CScript(PLAYERMGR)
	, m_pPlayer(nullptr)
{
	if (!m_This)
		m_This = this;
}

CPlayerMgr::~CPlayerMgr()
{

}

void CPlayerMgr::tick()
{
	if (!m_This)
		m_This = this;
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
		MessageBoxA(nullptr, "Can't Find Player Script From Player!", "Failed to Create Animation", MB_OK);
		assert(nullptr);
	}
}