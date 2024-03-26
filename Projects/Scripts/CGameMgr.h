#pragma once
#include <Engine/CScript.h>

// =======================================
// CGameMgr: Game 진행 관리
// =======================================

class CGameMgr :
    public CScript
{
private:
	static CGameMgr* m_This;

public:
	virtual void begin() override {};
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override {};
	virtual void LoadFromFile(FILE* _File) override {};

public:
	static CGameMgr* GameMgr() { return m_This; }

public:
	CLONE_DISABLE(CGameMgr)
	CGameMgr();
	~CGameMgr();
};

