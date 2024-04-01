#include "pch.h"
#include "CScytheDissScript.h"

CScytheDissScript::CScytheDissScript()
	: CScytheScript(SCYTHEDISSSCRIPT)
{
	Damage_ComboA = 10;
	Damage_ComboB = 12;
	Damage_ComboC = 8;
	Damage_ComboD = 15;
	Damage_Air = 20;
	Damage_Up = 12;
	Damage_Special = 10;
	Damage_Crush = 20;
	Damage_Concentrate1 = 25;
	Damage_Concentrate2 = 50;
	Damage_Rest = 50;

	SetName(L"ScytheDiss");
}

CScytheDissScript::~CScytheDissScript()
{
}
