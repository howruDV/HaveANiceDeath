#pragma once
#include "CUnitScript.h"

class CPlayerScript :
    public CUnitScript
{
private:
    // Info
    float m_fSpeedInAir;
    float m_fJumpVelocMax;

    // hp
    int m_iHPActive;

    // mp
    int m_iMPMax;
    int m_iMPCur;

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
    // --------------------------------------------------------------------

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    void SetHPActive(int _HPActive) { m_iHPActive = _HPActive; }
    void SetiMPMax(int _MPMax) { m_iMPMax = _MPMax; }
    void SetiMPCur(int _MPCur) { m_iMPCur = _MPCur; }
    void SetIngot(int _Ingot) { m_iIngot = _Ingot; }
    void SetSoulary(int _Soulary) { m_iSoulary = _Soulary; }
    void SetPrismium(int _Prismium) { m_iPrismium = _Prismium; }

    int GetHPActive() { return m_iHPActive; }
    int GetiMPMax() { return m_iMPMax; }
    int GetiMPCur() { return m_iMPCur; }
    int GetIngot() { return m_iIngot; }
    int GetSoulary() { return m_iSoulary; }
    int GetPrismium() { return m_iPrismium; }

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};