#pragma once
#include <Engine/CScript.h>

struct FDamage
{
    int iCurHPDamage;
    int iMaxHPDamage;
};

struct FAttackHistory
{
    CGameObject*    pTarget;
    float           fAccTime;
};

class CUnitScript;

class CHitboxScript :
    public CScript
{
private:
    vector<FAttackHistory> m_vecAttackHistory;
    FDamage                 m_Damage;
    int                     m_HostileLayer;
    float                   m_fCoolTime;
    bool                    m_bRepeat;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void Attack(CGameObject* pTarget);

public:
    void SetDamage(FDamage _Damage) { m_Damage = _Damage; }
    void SetCoolTime(float _Time) { m_fCoolTime = _Time; }
    void SetHostileLayer(int _LayerIdx) { m_HostileLayer = _LayerIdx; }
    void SetRepeat(bool _Repeat) { m_bRepeat = _Repeat; }

    FDamage GetDamage() { return m_Damage; }
    float GetCoolTime() { return m_fCoolTime; }
    int GetHostileLayer() { return m_HostileLayer; }
    bool IsRepeat() { return m_bRepeat; }

public:
    CLONE(CHitboxScript)
    CHitboxScript();
    CHitboxScript(const CHitboxScript& _Origin);
    ~CHitboxScript();
};