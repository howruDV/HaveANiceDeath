#pragma once
#include <Engine/CScript.h>

class CParallelScript :
    public CScript
{
private:
    Vec4 m_vColor;

public:
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(CParallelScript);
    CParallelScript();
    CParallelScript(const CParallelScript& _Origin);
    ~CParallelScript();
};