#pragma once
#include <Engine/CScript.h>

class CRotateScript :
    public CScript
{
private:
    float m_fSpeed;
    float m_fPos;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(CRotateScript);
    CRotateScript();
    CRotateScript(const CRotateScript& _Origin);
    ~CRotateScript();
};