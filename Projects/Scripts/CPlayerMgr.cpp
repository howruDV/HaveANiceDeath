#include "pch.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

#include <Engine/CGameObject.h>

CPlayerMgr::CPlayerMgr()
	:  m_pPlayer(nullptr)
{

}

CPlayerMgr::~CPlayerMgr()
{

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

	const vector<CScript*> vecScript = m_pPlayer->GetScripts();
	for (CScript* it : vecScript)
	{
		if (dynamic_cast<CPlayerScript*>(it))
		{
			m_pPlayerScript = dynamic_cast<CPlayerScript*>(it);
		}
	}

	if (not m_pPlayerScript)
	{
		MessageBoxA(nullptr, "Can't Find Player Script From Player!", "Failed to Create Animation", MB_OK);
		assert(nullptr);
	}
}