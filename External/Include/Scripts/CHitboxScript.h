#pragma once
#include <Engine/CScript.h>

class CUnitScript;

class CHitboxScript :
    public CScript
{
private:
    CUnitScript*    m_HitTarget;
    int             m_HostileLayer;
    int             m_iDamage;
    float           m_fCoolTime;
    float           m_fAccTime;
    bool            m_bRepeat;

public:
    virtual void begin() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetHostileLayer(int _LayerIdx) { m_HostileLayer = _LayerIdx; }
    void SetDamage(int _Damage) { m_iDamage = _Damage; }
    void SetCoolTime(float _Time) { m_fCoolTime = _Time; }
    void SetRepeat(bool _Repeat) { m_bRepeat = _Repeat; }

    int GetHostileLayer() { return m_HostileLayer; }
    int GetDamage() { return m_iDamage; }
    float GetCoolTime() { return m_fCoolTime; }
    bool IsRepeat() { return m_bRepeat; }

public:
    CLONE(CHitboxScript)
    CHitboxScript();
    CHitboxScript(const CHitboxScript& _Origin);
    ~CHitboxScript();
};