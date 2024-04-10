#pragma once
#include "CUnitScript.h"

class CHitboxScript;

class CMonsterScript :
    public CUnitScript
{
private:
    float   m_fDetectRange;
    float   m_fAttackRange;
    int     m_iAttackTypeCount;
    bool    m_bAppear;

    // state
    float   m_fAttackCoolTime;
    float   m_fAttackCoolAcc;
    bool    m_bAttackCool;
    bool    m_bFlying;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    void StartAttackCool() { m_bAttackCool = true; m_fAttackCoolAcc = 0.f; }

    int GetAttackTypeCount() { return m_iAttackTypeCount; }
    bool CanAttack() { return !m_bAttackCool; }

public:
    CLONE(CMonsterScript);
    CMonsterScript();
    CMonsterScript(const CMonsterScript& _Origin);
    ~CMonsterScript();
};