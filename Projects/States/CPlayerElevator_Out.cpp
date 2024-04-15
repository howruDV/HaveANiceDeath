#include "pch.h"
#include "CPlayerElevator_Out.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerElevator_Out::CPlayerElevator_Out()
	: CState(PLAYERELEVATOR_OUT)
{
}

CPlayerElevator_Out::~CPlayerElevator_Out()
{
}

void CPlayerElevator_Out::finaltick()
{
	// turn 중이면 state 변경 보류
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	ChangeState(L"Idle");
}

void CPlayerElevator_Out::Enter()
{
	// setting
	PLAYERSCRIPT->SetDirLock(true);

	// anim
	GetOwner()->Animator2D()->Play(L"Elevator_Out", false);
}

void CPlayerElevator_Out::Exit()
{
	PLAYERSCRIPT->SetDirLock(false);
}
