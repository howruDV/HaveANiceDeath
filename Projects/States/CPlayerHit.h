#pragma once
#include <Engine/CState.h>

class CPlayerHit :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CPlayerHit();
    ~CPlayerHit();
};