#pragma once
#include <Engine/CScript.h>

class CTransparentScript :
    public CScript
{
private:
    float m_TargetTrans;
    float m_Speed;
    bool m_FadeIn;
    bool m_AfterDestroy;

public:
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(CTransparentScript);
    CTransparentScript();
    CTransparentScript(const CTransparentScript& _Origin);
    ~CTransparentScript();
};