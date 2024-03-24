#pragma once
#include <Engine/singleton.h>

class CGameObject;
class CPlayerScript;

class CPlayerMgr
	: public CSingleton<CPlayerMgr>
{
	SINGLE(CPlayerMgr)

private:
	CGameObject* m_pPlayer;
	CPlayerScript* m_pPlayerScript;

public:
	//void SavePlayerInfo();

public:
	void SetPlayer(CGameObject* _obj);

	CGameObject* GetPlayer() { return m_pPlayer; };
	CPlayerScript* GetPlayerScript() { return m_pPlayerScript; }
};