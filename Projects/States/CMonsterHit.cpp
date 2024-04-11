#include "pch.h"
#include "CMonsterHit.h"

#include <Scripts\CMonsterScript.h>

CMonsterHit::CMonsterHit()
	: CState(MONSTERHIT)
{
}

CMonsterHit::~CMonsterHit()
{
}


void CMonsterHit::finaltick()
{
	// play animation
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	GetFSM()->SetGlobalState(false);
	ChangeState(L"Idle");
}

void CMonsterHit::Enter()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	MONSTERSCRIPT->SetDirLock(true);
	GetFSM()->SetGlobalState(true);

	GetOwner()->Animator2D()->Play(L"Hit", false);
}

void CMonsterHit::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
	GetFSM()->SetGlobalState(false);
}