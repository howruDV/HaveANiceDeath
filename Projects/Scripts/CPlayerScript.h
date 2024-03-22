#pragma once
#include <Engine/CScript.h>

class CPlayerScript :
    public CScript
{
    // hp
    int m_iHPMax;
    int m_iHPActive;
    int m_iHPCur;

    // mp
    int m_iMPMax;
    int m_iMPCur;

    // money
    int Ingot;
    int Soulary;
    int Prismium;

    //CAnimator* m_pAnimator;
    //CAnimator* m_pEffector;
    //CAnimator* m_pDustAnimator;
    //CAnimator* m_pSparkAnimator;
    //CCollider* m_pCollider;
    //CCollider* m_pHitBox;
    //CMovement* m_pMovement;
    //CStateMachine* m_pSM;
};

