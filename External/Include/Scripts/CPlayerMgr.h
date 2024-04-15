#pragma once
#include <Engine/CScript.h>

// =======================================
// CPlayerMgr: Player 추적, 기록
// =======================================

class CGameObject;
class CPlayerScript;
class CInvenMgr;

class CPlayerMgr
	: public CScript
{
private:
	static CGameObject*		m_pPlayer;
	static CPlayerScript*	m_pPlayerScript;
	static CInvenMgr*		m_pInvenScript;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override {};
	virtual void LoadFromFile(FILE* _File) override {};

	//void SavePlayerInfo();

public:
	void SetPlayer(CGameObject* _obj);

	static CGameObject* GetPlayer() { return m_pPlayer; }
	static CPlayerScript* GetPlayerScript() { return m_pPlayerScript; }
	static CInvenMgr* GetInventory() { return m_pInvenScript; }

public:
	CLONE_DISABLE(CPlayerMgr)
	CPlayerMgr();
	~CPlayerMgr();
};
