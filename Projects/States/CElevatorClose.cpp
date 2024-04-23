#include "pch.h"
#include "CElevatorClose.h"

#include <Engine\CStateMachine.h>

#include <Scripts\CPlayerMgr.h>
#include <Scripts\CPlayerScript.h>

CElevatorClose::CElevatorClose()
	: CState(ELEVATORCLOSE)
{
}

CElevatorClose::~CElevatorClose()
{
}

void CElevatorClose::finaltick()
{
}

void CElevatorClose::Enter()
{
	// sound
	GamePlayStatic::Play2DSound(L"sound\\obj\\elev\\GPE_Elevator_Close_Door_01.wav", 1, 0.3f);

	// Set Player
	CPlayerMgr::GetPlayerScript()->StateMachine()->GetFSM()->ChangeState(L"Elevator_In");
	CPlayerMgr::GetPlayer()->StateMachine()->AddBlackboardData(L"pLastInterElev", BB_DATA::OBJECT, GetOwner());
}

void CElevatorClose::Exit()
{
}