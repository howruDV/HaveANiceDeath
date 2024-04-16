#include "pch.h"
#include "func.h"

#include <Scripts\CPlayerMgr.h>
#include <Scripts\CPlayerScript.h>
#include <Scripts\CInvenMgr.h>

void SetPlayerHitboxDamage(CGameObject* _Obj, SCYTHE_DAMAGE_NAME _State)
{
	int Damage = GetScytheAttackDamage(_State);
	FDamage FDamage{ Damage,0 };

	_Obj->GetScriptByType<CHitboxScript>()->SetDamage(FDamage);
}

int GetScytheAttackDamage(SCYTHE_DAMAGE_NAME _State)
{
	SCYTHE_TYPE Type = INVENTORY->GetScytheType();
	int Damage = 0;
	
	switch (Type)
	{
	case SCYTHE_TYPE::DISS:
		Damage = *(int*)(&SCYTHE_DISS_DAMAGE + (sizeof(int) * (int)_State));
		break;
	}

	return Damage;
}