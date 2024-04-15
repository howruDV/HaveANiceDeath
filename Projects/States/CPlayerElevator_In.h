#pragma once
#include <Engine/CState.h>

class CPlayerElevator_In :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerElevator_In();
    ~CPlayerElevator_In();
};