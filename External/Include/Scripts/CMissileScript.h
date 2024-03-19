#pragma once

#include <Engine/CScript.h>

class CMissileScript :
    public CScript
{
private:
    float   m_fSpeed;

public:
    virtual void tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CMissileScript);
    CMissileScript();
    ~CMissileScript();
};

