#pragma once
#include <Engine/CScript.h>

class CScaleScript :
    public CScript
{
private:
    Vec3 m_OrgScale;
    Vec3 m_ScaleFactor;
    float m_ScaleSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(CScaleScript);
    CScaleScript();
    CScaleScript(const CScaleScript& _Origin);
    ~CScaleScript();
};