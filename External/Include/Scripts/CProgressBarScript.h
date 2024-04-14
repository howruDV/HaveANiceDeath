#pragma once
#include <Engine/CScript.h>
#include "deque"

enum class BAR
{
    LIFE,
    MANA,
};

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

class CPlayerScript;
class CGameObject;

class CProgressBarScript :
    public CScript
{
private:
    BAR m_Type;
    CPlayerScript* m_pUnit;
    deque<BAR_EVENT> m_queueEvent;
    float m_PrevDecAcc;
    float m_PrevIncAcc;

    CGameObject* m_BarL;
    CGameObject* m_BarM;
    CGameObject* m_BarR;
    CGameObject* m_BarM_Extra;
    CGameObject* m_BarR_Extra;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

    void Decrease(float _diff);
    void Increase(float _diff);

public:
    CLONE(CProgressBarScript);
    CProgressBarScript();
    ~CProgressBarScript();
};

