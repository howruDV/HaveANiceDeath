#pragma once
#include <Engine/CState.h>

class CKoTBigAttack1 :
    public CState
{
private:
    bool m_SoundPlay;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CKoTBigAttack1();
    ~CKoTBigAttack1();
};