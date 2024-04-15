#include "pch.h"
#include "CPlayerElevator_In.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerElevator_In::CPlayerElevator_In()
	: CState(PLAYERELEVATOR_IN)
{
}

CPlayerElevator_In::~CPlayerElevator_In()
{
}

void CPlayerElevator_In::finaltick()
{	
	// turn 중이면 state 변경 보류
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	// @TODO
}

void CPlayerElevator_In::Enter()
{
	// setting
	PLAYERSCRIPT->SetDirLock(true);
	GetFSM()->SetGlobalState(true);

	// anim
	GetOwner()->Animator2D()->Play(L"Elevator_Out", false, true);
}

void CPlayerElevator_In::Exit()
{
	PLAYERSCRIPT->SetDirLock(false);
	GetFSM()->SetGlobalState(false);
}
