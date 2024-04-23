#include "pch.h"
#include "CElevatorOpen.h"

#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>

#include <Scripts\CPlayerMgr.h>
#include <Scripts\CPlayerScript.h>

CElevatorOpen::CElevatorOpen()
	: CState(ELEVATOROPEN)
{
	m_pSound = CAssetMgr::GetInst()->Load<CSound>(L"sound\\obj\\elev\\GPE_Elevator_Open_Door_01.wav");
}

CElevatorOpen::~CElevatorOpen()
{
}

void CElevatorOpen::finaltick()
{
	ChangeState(L"Wait");
}

void CElevatorOpen::Enter()
{
	m_pSound->Play(1, 0.3f);

	// Set Player
	Vec3 Pos = CPlayerMgr::GetPlayer()->Transform()->GetRelativePos();
	Pos.x = GetOwner()->Transform()->GetWorldPos().x;

	CPlayerMgr::GetPlayer()->Activate();
	CPlayerMgr::GetPlayer()->Transform()->SetRelativePos(Pos);
	CPlayerMgr::GetPlayerScript()->StateMachine()->GetFSM()->ChangeState(L"Elevator_Out");
}

void CElevatorOpen::Exit()
{
}
