#pragma once
#include "CUnitScript.h"
// =======================================
// CPlayerScript: Player�� ������ ����, �����ϴ� Script
// =======================================
// - ����
// - ���� �Ǵܿ� ���Ǵ� ����

class CPlayerScript :
    public CUnitScript
{
private:
    // Info
    float m_fSpeedInAir;
    float m_fSpeedDash;
    float m_fJumpVelocMax;

    // hp
    int m_iHPActive;

    // mp
    int m_iMPMax;
    int m_iMPCur;

    // rest
    int m_iRestMax;
    int m_iRestCur;

    // cooltime
    float m_fDashCoolTime;
    float m_fDashAccTime;
    bool m_bDashCan;

    float m_fComboCoolTime;
    float m_fComboAccTime;
    int m_NextComboIdx;
    bool m_bComboCan;

    bool m_bAerialCan;

    // collision
    CGameObject* m_AirColPlatform;
    bool m_bAirCol;

    // @TODO : �κ��丮 ������ ���� �������ҵ�?----------------------------------
    // money
    int m_iIngot;
    int m_iSoulary;
    int m_iPrismium;

    // Anima
    int m_iAnimaMax;
    int m_iAnimaBlue;
    int m_iAnimaGold;

    // ����, ��������, ����
    // @TODO : ���⼭ �����ϴ°ͺ��� WeaponMgr ���� �� �ĵΰ� �����ϴ°� ������?
    // --------------------------------------------------------------------
    SCYTHE_TYPE m_CurScythe;


public:
    virtual void init() override;
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void BeginPushUp(CGameObject* _OtherObj) override;
    virtual void BeginPushDown(CGameObject* _OtherObj) override;
    virtual void EndPushUp(CGameObject* _OtherObj) override;
    virtual void EndPushDown(CGameObject* _OtherObj) override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

    virtual void HitDamage(FDamage _Damage) { CUnitScript::HitDamage(_Damage);  m_iHPActive -= _Damage.iActiveHPDamage; }
    virtual void Attack() { m_iRestCur += 5; }

public:
    void UseMP(int _value) { m_iMPCur -= _value; }
    void StartDashCoolTime(bool _bDashCan = false);
    void StartCombo(int _ComboIdx);

public:
    void SetHPActive(int _HPActive) { m_iHPActive = _HPActive; }
    void SetiMPMax(int _MPMax) { m_iMPMax = _MPMax; }
    void SetiMPCur(int _MPCur) { m_iMPCur = _MPCur; }
    void SetIngot(int _Ingot) { m_iIngot = _Ingot; }
    void SetSoulary(int _Soulary) { m_iSoulary = _Soulary; }
    void SetPrismium(int _Prismium) { m_iPrismium = _Prismium; }
    void SetScythe(SCYTHE_TYPE _Type) { m_CurScythe = _Type; }
    void DeactiveCombo() { m_bComboCan = false; m_fComboAccTime = 0; m_NextComboIdx = 0; }
    void DeactiveAerialAttack() { m_bAerialCan = false; }

    CGameObject* GetAirColPlatform() { return m_AirColPlatform; }
    SCYTHE_TYPE GetScytheType() { return m_CurScythe; }
    wstring GetScytheName() { return SCYTHE_NAME[(int)m_CurScythe]; }
    FScytheDamage GetScytheDamage();
    int GetHPActive() { return m_iHPActive; }
    int GetIngot() { return m_iIngot; }
    int GetSoulary() { return m_iSoulary; }
    int GetPrismium() { return m_iPrismium; }
    int GetNextComboIdx() { return m_NextComboIdx; }
    bool CanDash() { return m_bDashCan; }
    bool CanAerialAttack() { return m_bAerialCan; }
    bool CanUseMP(int _value) { return m_iMPCur <= _value; }
    bool IsRestFull() { return m_iRestCur == m_iRestMax; }
    bool IsAirCol() { return m_bAirCol; }

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    CPlayerScript(const CPlayerScript& _Origin);
    ~CPlayerScript();
};