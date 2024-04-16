#pragma once
#include <Engine/CState.h>

class CKoTFlyingAttack1 :
    public CState
{
private:
    bool m_SoundPlay[2];

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CKoTFlyingAttack1();
    ~CKoTFlyingAttack1();
};