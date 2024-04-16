#pragma once
#include <Engine/CScript.h>

class CGameObject;

class CTitleMgr :
    public CScript
{
private:
	CGameObject* m_Transition;
	bool m_GameStart;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override {};
	virtual void LoadFromFile(FILE* _File) override {};

public:
	CLONE_DISABLE(CTitleMgr)
	CTitleMgr();
	~CTitleMgr();
};