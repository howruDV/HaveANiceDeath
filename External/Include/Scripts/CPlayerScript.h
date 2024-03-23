#pragma once
#include <Engine/CScript.h>

class CPlayerScript :
    public CScript
{
private:
    float m_fSpeed;

    // hp
    int m_iHPMax;
    int m_iHPActive;
    int m_iHPCur;

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
    void init();
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    void SetHPMax(int _HPMax) { m_iHPMax = _HPMax; }
    void SetHPActive(int _HPActive) { m_iHPActive = _HPActive; }
    void SetHPCur(int _HPCur) { m_iHPCur = _HPCur; }
    void SetiMPMax(int _MPMax) { m_iMPMax = _MPMax; }
    void SetiMPCur(int _MPCur) { m_iMPCur = _MPCur; }
    void SetIngot(int _Ingot) { m_iIngot = _Ingot; }
    void SetSoulary(int _Soulary) { m_iSoulary = _Soulary; }
    void SetPrismium(int _Prismium) { m_iPrismium = _Prismium; }

    int GetHPMax(int _HPMax) { return m_iHPMax; }
    int GetHPActive(int _HPActive) { return m_iHPActive; }
    int GetHPCur(int _HPCur) { return m_iHPCur; }
    int GetiMPMax(int _MPMax) { return m_iMPMax; }
    int GetiMPCur(int _MPCur) { return m_iMPCur; }
    int GetIngot(int _Ingot) { return m_iIngot; }
    int GetSoulary(int _Soulary) { return m_iSoulary; }
    int GetPrismium(int _Prismium) { return m_iPrismium; }

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    CPlayerScript(CPlayerScript& _Origin);
    ~CPlayerScript();
};