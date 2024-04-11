#pragma once
#include <Engine/CState.h>

class CKoTSpearAttack2 :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CKoTSpearAttack2();
    ~CKoTSpearAttack2();
};