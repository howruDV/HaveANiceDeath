#pragma once
#include "CUnitScript.h"

class CScytheScript;

class CPlayerScript :
    public CUnitScript
{
private:
    // Info
    float m_fSpeedInAir;
    float m_fSpeedDash;
    float m_fJumpVelocMax;

    CGameObject* m_AirColPlatform;
    bool m_bAirCol;
    CGameObject* m_ColWall;
    bool m_bWallCol;

    // hp
    int m_iHPActive;

    // mp
    int m_iMPMax;
    int m_iMPCur;

    // cooltime
    float m_fDashCoolTime;
    float m_fDashAccTime;
    bool m_bDashCan;

    float m_fComboCoolTime;
    float m_fComboAccTime;
    int m_NextComboIdx;
    bool m_bComboCan;

    bool m_bAerialCan;

    // @TODO : 인벤토리 관리자 따로 만들어야할듯?----------------------------------
    // money
    int m_iIngot;
    int m_iSoulary;
    int m_iPrismium;

    // Anima
    int m_iAnimaMax;
    int m_iAnimaBlue;
    int m_iAnimaGold;

    // 무기, 보조무기, 저주
    // @TODO : 여기서 생성하는것보단 WeaponMgr 만들어서 쭉 파두고 선택하는게 좋을듯?
    // --------------------------------------------------------------------
    CScytheScript* m_CurScythe;


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void BeginPushUp(CGameObject* _OtherObj) override;
    virtual void BeginPushDown(CGameObject* _OtherObj) override;
    virtual void EndPushUp(CGameObject* _OtherObj) override;
    virtual void EndPushDown(CGameObject* _OtherObj) override;

    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

    void StartDashCoolTime(bool _bDashCan = false);
    void StartCombo(int _ComboIdx);

public:
    void SetHPActive(int _HPActive) { m_iHPActive = _HPActive; }
    void SetiMPMax(int _MPMax) { m_iMPMax = _MPMax; }
    void SetiMPCur(int _MPCur) { m_iMPCur = _MPCur; }
    void SetIngot(int _Ingot) { m_iIngot = _Ingot; }
    void SetSoulary(int _Soulary) { m_iSoulary = _Soulary; }
    void SetPrismium(int _Prismium) { m_iPrismium = _Prismium; }
    void DeactiveCombo() { m_bComboCan = false; m_fComboAccTime = 0; m_NextComboIdx = 0; }
    void DeactiveAerialAttack() { m_bAerialCan = false; }

    CGameObject* GetAirColPlatform() { return m_AirColPlatform; }
    CGameObject* GetColWall() { return m_ColWall; }
    CScytheScript* GetScythe() { return m_CurScythe; }
    int GetHPActive() { return m_iHPActive; }
    int GetiMPMax() { return m_iMPMax; }
    int GetiMPCur() { return m_iMPCur; }
    int GetIngot() { return m_iIngot; }
    int GetSoulary() { return m_iSoulary; }
    int GetPrismium() { return m_iPrismium; }
    int GetNextComboIdx() { return m_NextComboIdx; }
    bool CanDash() { return m_bDashCan; }
    bool CanAerialAttack() { return m_bAerialCan; }
    bool IsAirCol() { return m_bAirCol; }
    bool IsWallCol() { return m_bWallCol; }

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};