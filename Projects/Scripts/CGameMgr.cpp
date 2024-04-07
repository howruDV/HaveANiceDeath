#include "pch.h"
#include "CGameMgr.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

CGameMgr* CGameMgr::m_This = nullptr;

CGameMgr::CGameMgr()
	: CScript(GAMEMGR)
{
	m_This = this;
}

CGameMgr::~CGameMgr()
{
}

void CGameMgr::tick()
{
	if (CPlayerMgr::GetPlayerScript())
	{
		if (CPlayerMgr::GetPlayerScript()->IsDead())
		{
			// @TODO Game End
		}
	}
}