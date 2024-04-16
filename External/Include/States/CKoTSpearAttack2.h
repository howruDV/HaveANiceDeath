#pragma once
#include <Engine/CState.h>

class CKoTSpearAttack2 :
    public CState
{
private:
    bool m_SoundPlay[3];

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CKoTSpearAttack2();
    ~CKoTSpearAttack2();
};