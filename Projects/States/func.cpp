#include "pch.h"
#include "func.h"

#include <Scripts\CPlayerMgr.h>
#include <Scripts\CPlayerScript.h>

#include <random>

int Random(int min, int max)
{
	int ret = 0;
	std::random_device rd;  // true 랜덤 수 발생기
	std::mt19937 gen(rd()); // mersenne_twister_engine 알고리즘 사용
	std::uniform_int_distribution<int> dis(min, max); // 분포 설정

	return dis(gen); // 랜덤 정수 반환
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