#pragma once
#include <Engine/CScript.h>

// =======================================
// CGameMgr: Game 진행 관리
// =======================================

class CGameObject;
class CScript;

class CGameMgr :
    public CScript
{
private:
	static CGameObject* m_pMainCamera;
	CScript* m_pCurLevelMgr;
	//bool m_bTitle;
	//float m_bTitleAcc;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override {};
	virtual void LoadFromFile(FILE* _File) override {};

private:
	void GameEnding_Fail();
	void GameEnding_Win();
	void CreateLevelMgr(class CLevel* _Level);

public:
	static CGameObject* GetMainCamera() { return m_pMainCamera; }

public:
	CLONE_DISABLE(CGameMgr)
	CGameMgr();
	~CGameMgr();
};

