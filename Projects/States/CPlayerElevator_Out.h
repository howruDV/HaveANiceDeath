#pragma once
#include <Engine/CState.h>

class CPlayerElevator_Out :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerElevator_Out();
    ~CPlayerElevator_Out();
};

