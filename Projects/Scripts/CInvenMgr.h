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

class CGameObject;




class CInvenMgr :
    public CScript
{
private:
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
    void SetIngot(int _Ingot) { m_iIngot = _Ingot; }
    void SetSoulary(int _Soulary) { m_iSoulary = _Soulary; }
    void SetPrismium(int _Prismium) { m_iPrismium = _Prismium; }

    int GetIngot() { return m_iIngot; }
    int GetSoulary() { return m_iSoulary; }
    int GetPrismium() { return m_iPrismium; }

public:
	CLONE_DISABLE(CInvenMgr);
	CInvenMgr();
	~CInvenMgr();
};

