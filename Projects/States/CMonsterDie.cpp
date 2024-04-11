#include "pch.h"
#include "CMonsterDie.h"

#include <Scripts\CMonsterScript.h>

CMonsterDie::CMonsterDie()
	: CState(MONSTERDIE)
{
}

CMonsterDie::~CMonsterDie()
{
}

void CMonsterDie::finaltick()
{
	// play animation
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	GamePlayStatic::DestroyGameObject(GetOwner());
}

void CMonsterDie::Enter()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	MONSTERSCRIPT->SetDirLock(true);
	GetFSM()->SetGlobalState(true);

	GetOwner()->Animator2D()->Play(L"Die", false);
}

void CMonsterDie::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
}