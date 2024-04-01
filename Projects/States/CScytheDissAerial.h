#pragma once
#include <Engine/CState.h>

class CScytheDissAerial :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScytheDissAerial();
    ~CScytheDissAerial();
};