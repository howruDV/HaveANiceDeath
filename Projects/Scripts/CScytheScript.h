#pragma once
#include <Engine/CScript.h>

class CScytheScript :
    public CScript
{
protected:
    int Damage_ComboA;
    int Damage_ComboB;
    int Damage_ComboC;
    int Damage_ComboD;
    int Damage_Air;
    int Damage_Up;
    int Damage_Special;
    int Damage_Crush;
    int Damage_Concentrate1;
    int Damage_Concentrate2;
    int Damage_Rest;

public:
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    CLONE(CScytheScript)
    CScytheScript(UINT m_iScriptType);
    ~CScytheScript();
};

