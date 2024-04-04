#pragma once
#include <Engine/CScript.h>
#include "CHitboxScript.h"

class CGameObject;

class CUnitScript :
    public CScript
{
protected:
    // Info
    float       m_fSpeed;
    UNIT_DIRX   m_Dir;
    UNIT_DIRX   m_Dir_Prev;
    UNIT_DIRX   m_Dir_Next;
    bool        m_bDirLock;
    bool        m_bDirChange_Next;
    bool        m_bDirChange_Cur;

    // Collision
    vector<CGameObject*> m_CollisionGround;

    // hp
    int         m_iHPMax;
    int         m_iHPCur;

public:
    virtual void begin() override {};
    virtual void tick() override;

    //virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override {}
    //virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void BeginPushUp(CGameObject* _OtherObj) {};
    virtual void BeginPushDown(CGameObject* _OtherObj) {};
    virtual void BeginPushLeft(CGameObject* _OtherObj) {};
    virtual void BeginPushRight(CGameObject* _OtherObj) {};
    virtual void EndPushUp(CGameObject* _OtherObj) {};
    virtual void EndPushDown(CGameObject* _OtherObj) {};
    virtual void EndPushLeft(CGameObject* _OtherObj) {};
    virtual void EndPushRight(CGameObject* _OtherObj) {};

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

    // Hit & Attack
    void HitDamage(FDamage _Damage) { m_iHPCur -= _Damage.iCurHPDamage; m_iHPMax -= _Damage.iMaxHPDamage; }
    //void AddDamage(float iDamage);

public:
    void SetSpeed(float _fSpeed) { m_fSpeed = m_fSpeed; }
    void SetDir(UNIT_DIRX _dir) { if(!m_bDirLock) m_Dir_Next = _dir; }
    void SetHPMax(int _iHP) { m_iHPMax = _iHP; }
    void SetHPCur(int _iHP) { m_iHPCur = _iHP; }
    void SetDirLock(bool _bLock) { m_bDirLock = _bLock; }
    void PushGround(CGameObject* _Ground) { m_CollisionGround.push_back(_Ground); }
    void DeleteGround(CGameObject* _Ground);

    float GetSpeed() { return m_fSpeed; }
    UNIT_DIRX GetDir() { return m_Dir_Prev; }
    int GetHPMax() { return m_iHPMax; }
    int GetHPCur() { return m_iHPCur; }
    int GetGroundCount() { return (int)m_CollisionGround.size(); }
    bool IsDead() { return m_iHPCur <= 0; }
    bool WillDirChange() { return m_bDirChange_Next; }
    bool IsDirChange() { return m_bDirChange_Cur; }
    bool IsDirLock() { return m_bDirLock; }
    bool IsGround(CGameObject* _Platform);

public:
    CLONE(CUnitScript);
    CUnitScript(UINT m_iScriptType);
    CUnitScript(const CUnitScript& _Origin);
    ~CUnitScript();
};