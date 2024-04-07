#include "pch.h"
#include "CPlayerHit.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerHit::CPlayerHit()
	: CState(PLAYERHIT)
{
}

CPlayerHit::~CPlayerHit()
{
}

void CPlayerHit::finaltick()
{
	// turn 중이면 state 변경 보류
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	GetFSM()->SetGlobalState(false);
	ChangeState(L"Idle");
}

void CPlayerHit::Enter()
{
	// setting
	GetOwner()->Movement()->SetVelocity(Vec3());
	PLAYERSCRIPT->SetDirLock(true);
	GetFSM()->SetGlobalState(true);

	// anim
	GetOwner()->Animator2D()->Play(L"Hit", false);
}

void CPlayerHit::Exit()
{
	PLAYERSCRIPT->SetDirLock(false);
}