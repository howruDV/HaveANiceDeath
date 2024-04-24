#pragma once
#include <Engine/CScript.h>

class CGameMgr;

class CLevelMgrScript_W09_Field1 :
    public CScript
{
private:
	CGameMgr* m_GameMgr;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override {};
	virtual void LoadFromFile(FILE* _File) override {};

	static Vec3 CheckCamArea(Vec3 _Pos);

private:
	//void GameEnding_Fail();

public:
	void SetGameMgr(CGameMgr* _Script) { m_GameMgr = _Script; }

public:
	CLONE_DISABLE(CLevelMgrScript_W09_Field1)
	CLevelMgrScript_W09_Field1();
	~CLevelMgrScript_W09_Field1();
};