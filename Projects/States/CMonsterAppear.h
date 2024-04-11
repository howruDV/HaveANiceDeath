#pragma once
#include <Engine/CState.h>

class CMonsterAppear :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CMonsterAppear();
    ~CMonsterAppear();
};