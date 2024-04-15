#pragma once
#include <Engine/CScript.h>
#include "deque"

enum class BAR_EVENT_TYPE
{
    DEC_BAR,
    INC_BAR,
    DEC_EXTRA,
    INC_EXTRA,
};

struct BAR_EVENT
{
    BAR_EVENT_TYPE Type;
    float PlayTIme;
    float AccTime;
    float Diff;
};

class CUnitScript;
class CGameObject;

class CProgressBarScript :
    public CScript
{
private:
    deque<BAR_EVENT>    m_queueEvent;
    float               m_PrevDecAcc;
    float               m_PrevIncAcc;

    CGameObject*        m_BarL;
    CGameObject*        m_BarM;
    CGameObject*        m_BarR;
    CGameObject*        m_BarM_Extra;
    CGameObject*        m_BarR_Extra;

protected:
    CUnitScript*        m_pUnit;
    float               m_fWidth;
    float               m_fDecTime;
    float               m_fIncTime;
    float               m_fProgressSpeed;
    Vec4                m_DefaultColor;
    Vec4                m_DecColor;
    Vec4                m_IncColor;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

    void SetProgress(float _progress);
    void Decrease(float _diff);
    void Increase(float _diff);

protected:
    virtual float GetProgress() { return 0.f; }

public:
    CLONE(CProgressBarScript);
    CProgressBarScript(UINT _ScriptType);
    ~CProgressBarScript();
};

