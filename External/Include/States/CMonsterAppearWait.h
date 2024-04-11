#pragma once
#include <Engine/CState.h>

class CMonsterAppearWait :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CMonsterAppearWait();
    ~CMonsterAppearWait();
};