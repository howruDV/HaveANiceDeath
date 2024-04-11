#pragma once
#include <Engine/CState.h>
#include <Engine\CPrefab.h>

class CKoTSpearAttack1 :
    public CState
{
private:
    Ptr<CPrefab> m_pProj;
    bool m_bThrow;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void InstanceProj();

public:
    CKoTSpearAttack1();
    ~CKoTSpearAttack1();
};