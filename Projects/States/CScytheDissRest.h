#pragma once
#include <Engine/CState.h>

class CScytheDissRest :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScytheDissRest();
    ~CScytheDissRest();
};