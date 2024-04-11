#include "pch.h"
#include "CMonsterAppear.h"

#include <Scripts\CMonsterScript.h>

CMonsterAppear::CMonsterAppear()
	: CState(MONSTERAPPEAR)
{
}

CMonsterAppear::~CMonsterAppear()
{
}

void CMonsterAppear::finaltick()
{
	// play animation
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	ChangeState(L"Idle");
}

void CMonsterAppear::Enter()
{
	MONSTERSCRIPT->SetDirLock(true);
	GetOwner()->Animator2D()->Play(L"Appear", false);
}

void CMonsterAppear::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
}