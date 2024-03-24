#pragma once
#include <Engine/CScript.h>

class CPlayerScript :
    public CScript
{
private:
    // Info
    float m_fSpeed;
    bool m_bLookLeft;
    bool m_bLookLeft_Prev;

    // hp
    int m_iHPMax;
    int m_iHPActive;
    int m_iHPCur;

    // mp
    int m_iMPMax;
    int m_iMPCur;

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
    void SetHPMax(int _HPMax) { m_iHPMax = _HPMax; }
    void SetHPActive(int _HPActive) { m_iHPActive = _HPActive; }
    void SetHPCur(int _HPCur) { m_iHPCur = _HPCur; }
    void SetiMPMax(int _MPMax) { m_iMPMax = _MPMax; }
    void SetiMPCur(int _MPCur) { m_iMPCur = _MPCur; }
    void SetIngot(int _Ingot) { m_iIngot = _Ingot; }
    void SetSoulary(int _Soulary) { m_iSoulary = _Soulary; }
    void SetPrismium(int _Prismium) { m_iPrismium = _Prismium; }
    void SetLookLeft(bool _LookLeft) { m_bLookLeft = _LookLeft; }

    int GetHPMax() { return m_iHPMax; }
    int GetHPActive() { return m_iHPActive; }
    int GetHPCur() { return m_iHPCur; }
    int GetiMPMax() { return m_iMPMax; }
    int GetiMPCur() { return m_iMPCur; }
    int GetIngot() { return m_iIngot; }
    int GetSoulary() { return m_iSoulary; }
    int GetPrismium() { return m_iPrismium; }
    bool GetLookLeft() { return m_bLookLeft; }

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    CPlayerScript(CPlayerScript& _Origin);
    ~CPlayerScript();
};