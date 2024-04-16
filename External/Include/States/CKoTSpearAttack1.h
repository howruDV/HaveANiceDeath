#pragma once
#include <Engine/CState.h>
#include <Engine\CPrefab.h>

class CKoTSpearAttack1 :
    public CState
{
private:
    bool m_SoundPlay[2];
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