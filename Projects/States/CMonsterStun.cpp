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
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	GetFSM()->SetGlobalState(false);
	ChangeState(L"Idle");
}

void CMonsterStun::Enter()
{
	MONSTERSCRIPT->SetDirLock(true);
	//MONSTERSCRIPT->StartStun();
	GetFSM()->SetGlobalState(true);

	GetOwner()->Animator2D()->Play(L"Stun", false);
}

void CMonsterStun::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
}