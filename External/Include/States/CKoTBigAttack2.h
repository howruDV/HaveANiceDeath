#pragma once
#include <Engine/CState.h>

class CKoTBigAttack2 :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CKoTBigAttack2();
    ~CKoTBigAttack2();
};