#pragma once
#include "CUnitScript.h"
// =======================================
// CPlayerScript: Player의 정보를 정의, 관리하는 Script
// =======================================
// - 스탯
// - 상태 판단에 사용되는 정보

class CProgressBarScript;

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
    CProgressBarScript* m_HPbar_Cur;
    CProgressBarScript* m_HPbar_Active;

    // mp
    int m_iMPMax;
    int m_iMPCur;
    CProgressBarScript* m_MPbar;

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

    // 무기, 보조무기, 저주
    // @TODO : 여기서 생성하는것보단 WeaponMgr 만들어서 쭉 파두고 선택하는게 좋을듯?
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

    virtual void HitDamage(FDamage _Damage);
    virtual void Attack() { m_iRestCur += 5; }

    void AddHPCur(int _Add);
    void AddHPActive(int _Add);

public:
    void UseMP(int _value) { m_iMPCur -= _value; }
    void StartDashCoolTime(bool _bDashCan = false);
    void StartCombo(int _ComboIdx);

public:
    void SetHPActive(int _HPActive) { m_iHPActive = _HPActive; }
    void SetiMPMax(int _MPMax) { m_iMPMax = _MPMax; }
    void SetiMPCur(int _MPCur) { m_iMPCur = _MPCur; }
    void SetScythe(SCYTHE_TYPE _Type) { m_CurScythe = _Type; }
    void DeactiveCombo() { m_bComboCan = false; m_fComboAccTime = 0; m_NextComboIdx = 0; }
    void DeactiveAerialAttack() { m_bAerialCan = false; }

    CGameObject* GetAirColPlatform() { return m_AirColPlatform; }
    SCYTHE_TYPE GetScytheType() { return m_CurScythe; }
    wstring GetScytheName() { return SCYTHE_NAME[(int)m_CurScythe]; }
    FScytheDamage GetScytheDamage();
    int GetMPCur() { return m_iMPCur; }
    int GetMPMax() { return m_iMPMax; }
    int GetHPActive() { return m_iHPActive; }
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