#pragma once
#include <Engine/CScript.h>

enum class ANIMA_TYPE
{
    NONE,
    BLUE,
    YELLOW,
};

struct FAnima
{
    ANIMA_TYPE Type;
    CGameObject* pObj;
};

struct FWeapon
{
    int Type;
    CGameObject* pObj;
};

class CGameObject;




class CInvenMgr :
    public CScript
{
private:
    // weapon
    FWeapon m_Weapon[3];

    // anima
    FAnima m_Anima[3];
    Ptr<CMaterial> m_AnimaMat[2];
    int m_iAnimaCount;

    // money
    int m_iIngot;
    int m_iSoulary;
    int m_iPrismium;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override {};
	virtual void LoadFromFile(FILE* _File) override {};

    bool UseAnima();
    void AddAnima();

public:
    void SetScythe(SCYTHE_TYPE _Type) { m_Weapon[0].Type = (int)_Type; }
    //void SetWeapon(int _Idx, SCYTHE_TYPE _Type) { if (_Idx <= 0 || _Idx > 2) return; m_Weapon[_Idx].Type = (int)_Type; m_Weapon[_idx].pObj->Activate();}
    void SetIngot(int _Ingot) { m_iIngot = _Ingot; }
    void SetSoulary(int _Soulary) { m_iSoulary = _Soulary; }
    void SetPrismium(int _Prismium) { m_iPrismium = _Prismium; }

    SCYTHE_TYPE GetScytheType() { return (SCYTHE_TYPE)m_Weapon[0].Type; }
    //SCYTHE_TYPE GetWeaponType(int _Idx) { if (_Idx <= 0 || _Idx > 2) return -1; return (SCYTHE_TYPE)m_Weapon[_Idx].Type; }
    wstring GetScytheName() { return SCYTHE_NAME[m_Weapon[0].Type]; }
    //wstring GetWeaponName(int _Idx) { if (_Idx <= 0 || _Idx > 2) return L""; return SCYTHE_NAME[m_Weapon[_Idx].Type]; }
    FScytheDamage GetScytheDamage();
    int GetIngot() { return m_iIngot; }
    int GetSoulary() { return m_iSoulary; }
    int GetPrismium() { return m_iPrismium; }

public:
	CLONE_DISABLE(CInvenMgr);
	CInvenMgr();
	~CInvenMgr();
};

