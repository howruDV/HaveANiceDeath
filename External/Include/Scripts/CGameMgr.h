#pragma once
#include <Engine/CScript.h>

// =======================================
// CGameMgr: Game 진행 관리
// =======================================

class CGameObject;

class CGameMgr :
    public CScript
{
private:
	CGameObject* m_GameEnding_Fail;

public:
	virtual void begin() override {};
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override {};
	virtual void LoadFromFile(FILE* _File) override {};

private:
	void GameEnding_Fail();
	void GameEnding_Win();

public:
	void SetGameEndingFail(CGameObject* pObj) { m_GameEnding_Fail = pObj; }

public:
	CLONE_DISABLE(CGameMgr)
	CGameMgr();
	~CGameMgr();
};

