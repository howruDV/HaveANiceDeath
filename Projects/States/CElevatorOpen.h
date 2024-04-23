#pragma once
#include <Engine/CState.h>
#include <Engine\CSound.h>

class CElevatorOpen :
    public CState
{
private:
    Ptr<CSound> m_pSound;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CElevatorOpen();
    ~CElevatorOpen();
};