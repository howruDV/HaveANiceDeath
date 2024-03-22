#pragma once
#include <Engine/CScript.h>
#include <Engine/CPrefab.h>

// =======================================
// CPlayerScript: Player ±¸ÇöºÎ
// =======================================
// imp01. player controller

class CPlayerScript_Test :
    public CScript
{
private:
    Ptr<CPrefab>    m_Missile;
    float           m_Speed;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CPlayerScript_Test);
    CPlayerScript_Test();
    ~CPlayerScript_Test();
};