#pragma once
#include <Engine/CScript.h>

// =======================================
// CPlayerMgr: Player 추적, 기록
// =======================================

class CGameObject;
class CPlayerScript;

class CPlayerMgr
	: public CScript
{
private:
	static CPlayerMgr* m_This;
	CGameObject*	m_pPlayer;
	CPlayerScript*	m_pPlayerScript;

public:
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override {};
	virtual void LoadFromFile(FILE* _File) override {};

	//void SavePlayerInfo();

public:
	static CPlayerMgr* PlayerMgr() { return m_This; }
	void SetPlayer(CGameObject* _obj);

	CGameObject* GetPlayer() { return m_pPlayer; };
	CPlayerScript* GetPlayerScript() { return m_pPlayerScript; }

public:
	CLONE_DISABLE(CPlayerMgr)
	CPlayerMgr();
	~CPlayerMgr();
};
