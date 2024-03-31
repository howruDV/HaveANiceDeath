#pragma once
#include <Engine/CState.h>

class CPlayerConcentrate :
    public CState
{
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerConcentrate();
    ~CPlayerConcentrate();
};