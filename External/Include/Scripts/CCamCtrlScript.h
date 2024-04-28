#pragma once
#include <Engine/CScript.h>
#include <deque>

typedef Vec3 (*DELEGATE_CAMCTRL)(Vec3);

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
    float fPlayTime = 0.f;
    float fAccTime = 0.f;
    float fVar = 0.f;
};

class CGameObejct;

class CCamCtrlScript :
    public CScript
{
private:
    // delegate
    static DELEGATE_CAMCTRL CheckCamArea;

    // target follow
    CGameObject*        m_Target;
    Vec3                m_vTargetOffset;
    Vec3                m_vMove;
    float               m_fSpeed;
    float               m_fSpeedScale;
    bool                m_IsTracking;

    // effect
    deque<FCamEffect>   m_queueEffect;
    CGameObject*        m_Transition;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    void SetTarget(CGameObject* _Target) { m_Target = _Target; }
    void SetEffect(FCamEffect _Effect);
    void PushEffect(FCamEffect _Effect);
    void PushTransition(bool _Start);
    void PopEffect();
    void SetCheckCamArea(DELEGATE_CAMCTRL _address) { CheckCamArea = _address; }

    Vec3 GetMove() { return m_vMove; }

public:
    CLONE(CCamCtrlScript);
    CCamCtrlScript();
    CCamCtrlScript(const CCamCtrlScript& _Origin);
    ~CCamCtrlScript();
};