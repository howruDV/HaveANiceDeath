#include "pch.h"
#include "CPlayerElevator_In.h"

#include <Engine\CStateMachine.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerElevator_In::CPlayerElevator_In()
	: CState(PLAYERELEVATOR_IN)
	, m_bFirst(true)
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

	if (m_bFirst)
	{
		CGameObject* pLastInterElev = (CGameObject*)GetBlackboardData(L"pLastInterElev");
		pLastInterElev->StateMachine()->GetFSM()->ChangeState(L"Disappear");
		m_bFirst = false;
	}
}

void CPlayerElevator_In::Enter()
{
	// setting
	PLAYERSCRIPT->SetDirLock(true);
	GetFSM()->SetGlobalState(true);
	m_bFirst = true;

	// anim
	GetOwner()->Animator2D()->Play(L"Elevator_Out", false, true);
}

void CPlayerElevator_In::Exit()
{
	PLAYERSCRIPT->SetDirLock(false);
	GetFSM()->SetGlobalState(false);
}
