#pragma once
#include <Engine/CState.h>

class CEmpty :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CEmpty();
    ~CEmpty();
};