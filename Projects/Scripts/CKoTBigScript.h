#pragma once
#include "CUnitScript.h"

class CKoTBigScript :
    public CUnitScript
{
private:
    float       m_fDetectRange;
    int         m_iAttack2Damage;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(CKoTBigScript);
    CKoTBigScript();
    ~CKoTBigScript();
};