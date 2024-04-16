#pragma once
#include <Engine/CState.h>

class CKoTSpearAttack3 :
    public CState
{
private:
    bool m_SoundPlay[3];

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CKoTSpearAttack3();
    ~CKoTSpearAttack3();
};