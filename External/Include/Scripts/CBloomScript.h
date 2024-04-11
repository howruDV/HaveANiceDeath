#pragma once
#include <Engine/CScript.h>

class CBloomScript :
    public CScript
{
private:
    bool m_bUse;
    Vec4 m_BloomColor;
    float m_fThreshold;

public:
    virtual void begin();

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(CBloomScript);
    CBloomScript();
    CBloomScript(const CBloomScript& _Origin);
    ~CBloomScript();
};

