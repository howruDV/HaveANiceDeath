#pragma once
#include <Engine/CScript.h>

class CGaugeScript :
    public CScript
{
private:
    float m_iCurVar;        // current gauge percent
    float m_iTargetVar;     // target gauge percent

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    void SetGauge(float _progress);
    void Increase(float _progress);
    void Reset();

public:
    CLONE(CGaugeScript);
    CGaugeScript();
    ~CGaugeScript();
};