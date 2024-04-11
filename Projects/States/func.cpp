#include "pch.h"
#include "func.h"

#include <Scripts\CPlayerMgr.h>
#include <Scripts\CPlayerScript.h>

#include <random>

int Random(int min, int max)
{
	int ret = 0;
	std::random_device rd;  // true ���� �� �߻���
	std::mt19937 gen(rd()); // mersenne_twister_engine �˰��� ���
	std::uniform_int_distribution<int> dis(min, max); // ���� ����

	return dis(gen); // ���� ���� ��ȯ
}

void SetPlayerHitboxDamage(CGameObject* _Obj, SCYTHE_DAMAGE_NAME _State)
{
	int Damage = GetScytheAttackDamage(_State);
	FDamage FDamage{ Damage,0 };

	_Obj->GetScriptByType<CHitboxScript>()->SetDamage(FDamage);
}

int GetScytheAttackDamage(SCYTHE_DAMAGE_NAME _State)
{
	SCYTHE_TYPE Type = PLAYERSCRIPT->GetScytheType();
	int Damage = 0;
	
	switch (Type)
	{
	case SCYTHE_TYPE::DISS:
		Damage = *(int*)(&SCYTHE_DISS_DAMAGE + (sizeof(int) * (int)_State));
		break;
	}

	return Damage;
}