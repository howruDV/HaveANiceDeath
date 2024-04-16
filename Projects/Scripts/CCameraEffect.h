#pragma once
#include <Engine/CScript.h>
#include <deque>

//enum class CAMEFFECT_TYPE
//{
//    SHAKE,
//    TRANSITION_OFF,
//    TRANSITION_ON,
//    END,
//};
//
//struct FCamEffect
//{
//    CAMEFFECT_TYPE Type;
//    float fPlayTime;
//    float fAccTime;
//    float fVar;
//};

class CCameraEffect :
    public CScript
{
private:
    //deque<FCamEffect> m_queueEffect;

public:
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

    //void PushEffect(FCamEffect _Effect) { m_queueEffect.push_back(_Effect); }

public:
    CLONE(CCameraEffect);
    CCameraEffect();
    ~CCameraEffect();
};