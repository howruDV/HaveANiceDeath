#pragma once
#include <Engine/CScript.h>

class CWallScript :
    public CScript
{
private:
    bool m_bUpdownCollision;
    bool m_bSideCollision;
    float m_fGroundAdjust;
    float m_fSideAdjust;

public:
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetUpdownCollision(bool _bCol) { m_bUpdownCollision = _bCol; }
    void SetSideCollision(bool _bCol) { m_bSideCollision = _bCol; }

public:
    CLONE(CWallScript)
    CWallScript();
    CWallScript(const CWallScript& _Origin);
    ~CWallScript();
};