#pragma once
#include <Engine/CScript.h>

class CCamCtrlScript;

class CParallelScript :
    public CScript
{
private:
    CCamCtrlScript* m_MainCamCtrlr;
    float m_fSpeed;

    // air perspective
    Vec4 m_vAirColor;
    bool m_bAirPerspective;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(CParallelScript);
    CParallelScript();
    CParallelScript(const CParallelScript& _Origin);
    ~CParallelScript();
};