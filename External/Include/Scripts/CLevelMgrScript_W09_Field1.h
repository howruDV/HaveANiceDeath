#pragma once
#include <Engine/CScript.h>

class CLevelMgrScript_W09_Field1 :
    public CScript
{
public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override {};
	virtual void LoadFromFile(FILE* _File) override {};

	static Vec3 CheckCamArea(Vec3 _Pos);

private:
	//void GameEnding_Fail();

public:
	CLONE_DISABLE(CLevelMgrScript_W09_Field1)
	CLevelMgrScript_W09_Field1();
	~CLevelMgrScript_W09_Field1();
};