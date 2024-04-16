#pragma once
#include <Engine/CScript.h>
#include <deque>

enum class CAMEFFECT_TYPE
{
    SHAKE,
    TRANSITION_OFF,
    TRANSITION_ON,
    END,
};

struct FCamEffect
{
    CAMEFFECT_TYPE Type;
    float fPlayTime;
    float fAccTime;
    float fVar;
};

class CGameObejct;

class CCamCtrlScript :
    public CScript
{
private:
    CGameObject* m_Target;
    Vec3 m_vPrevPos;
    Vec3 m_vMove;
    float m_fSpeed;

    deque<FCamEffect> m_queueEffect;
    CGameObject* m_Transition;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    void SetTarget(CGameObject* _Target) { m_Target = _Target; }
    void PushEffect(FCamEffect _Effect) { m_queueEffect.push_back(_Effect); }
    void PushTransition(bool _Start);

    Vec3 GetMove() { return m_vMove; }

private:
    Vec3 CheckCamArea(Vec3 _Pos);

public:
    CLONE(CCamCtrlScript);
    CCamCtrlScript();
    CCamCtrlScript(const CCamCtrlScript& _Origin);
    ~CCamCtrlScript();
};