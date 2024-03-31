#pragma once
#include <Engine/CState.h>

class CPlayerConcentrate_Start :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerConcentrate_Start();
    ~CPlayerConcentrate_Start();
};