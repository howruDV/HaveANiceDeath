#pragma once
#include <Engine/CState.h>

class CGameObject;

class CKoTBigAttack1 :
    public CState
{
    private:
        CGameObject * m_pHitbox;

    public:
        virtual void finaltick() override;
        virtual void Enter() override;
        virtual void Exit() override;

    public:
        CKoTBigAttack1();
        ~CKoTBigAttack1();
};