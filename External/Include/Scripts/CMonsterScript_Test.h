#pragma once
#include <Engine/CScript.h>

class CMonsterScript_Test :
    public CScript
{
private:
    float       m_DetectRange;
    float       m_Speed;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CMonsterScript_Test);

public:
    CMonsterScript_Test();
    ~CMonsterScript_Test();
};

