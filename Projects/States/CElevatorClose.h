#pragma once
#include <Engine/CState.h>

class CElevatorClose :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CElevatorClose();
    ~CElevatorClose();
};