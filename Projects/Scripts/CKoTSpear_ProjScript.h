#pragma once
#include <Engine/CScript.h>

class CKoTSpear_ProjScript :
    public CScript
{
private:
    UNIT_DIRX   m_Dir;
    float       m_fSpeed;
    float       m_fAccTime;
    float       m_fLifeTime;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetDir(UNIT_DIRX _Dir) { m_Dir = _Dir; }

public:
    CLONE(CKoTSpear_ProjScript);
    CKoTSpear_ProjScript();
    ~CKoTSpear_ProjScript();
};