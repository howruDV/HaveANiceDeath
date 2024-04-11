#include "pch.h"
#include "CMonsterStun.h"

#include <Scripts\CMonsterScript.h>

CMonsterStun::CMonsterStun()
	: CState(MONSTERSTUN)
{
}

CMonsterStun::~CMonsterStun()
{
}

void CMonsterStun::finaltick()
{
	// playing animation
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	ChangeState(L"Idle");
}

void CMonsterStun::Enter()
{
	MONSTERSCRIPT->SetDirLock(true);
	GetOwner()->Animator2D()->Play(L"Stun", false);
}

void CMonsterStun::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
}