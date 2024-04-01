#include "pch.h"
#include "CScytheScript.h"

CScytheScript::CScytheScript(UINT m_iScriptType)
	: CScript(m_iScriptType)
	, Damage_ComboA(0)
	, Damage_ComboB(0)
	, Damage_ComboC(0)
	, Damage_ComboD(0)
	, Damage_Air(0)
	, Damage_Up(0)
	, Damage_Special(0)
	, Damage_Crush(0)
	, Damage_Concentrate1(0)
	, Damage_Concentrate2(0)
	, Damage_Rest(0)
{
}

CScytheScript::~CScytheScript()
{
}

void CScytheScript::tick()
{
}

