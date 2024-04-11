#pragma once
#include <Engine/CState.h>

class CGameObject;

class CKoTBigAttack2 :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void UpdateHitboxTransform(CGameObject* Hitbox);

public:
    CKoTBigAttack2();
    ~CKoTBigAttack2();
};