#pragma once
#include <Engine/CState.h>

class CMonsterIdle :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CMonsterIdle();
    ~CMonsterIdle();
};