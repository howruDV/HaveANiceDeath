#pragma once
#include <Engine/CState.h>

class CMonsterDie :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CMonsterDie();
    ~CMonsterDie();
};