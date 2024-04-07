#include "pch.h"
#include "CPlayerDie.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerDie::CPlayerDie()
	: CState(PLAYERDIE)
{
}

CPlayerDie::~CPlayerDie()
{
}

void CPlayerDie::finaltick()
{
	// turn 중이면 state 변경 보류
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	// @TODO : PlayerGameOver
}

void CPlayerDie::Enter()
{
	// setting
	GetOwner()->Movement()->SetVelocity(Vec3());
	PLAYERSCRIPT->SetDirLock(true);
	GetFSM()->SetGlobalState(true);

	// anim
	GetOwner()->Animator2D()->Play(L"Die", false);
}

void CPlayerDie::Exit()
{
	PLAYERSCRIPT->SetDirLock(false);
	GetFSM()->SetGlobalState(false);
}