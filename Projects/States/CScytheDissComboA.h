#pragma once
#include <Engine/CState.h>

class CScytheDissComboA :
    public CState
{
private:
    bool m_bCombo;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScytheDissComboA();
    ~CScytheDissComboA();
};