#include "pch.h"
#include "CElevatorAppear.h"

#include <Engine\CAnimator2D.h>
#include <Engine\CTransform.h>

#include <Scripts\CPlayerMgr.h>
#include <Scripts\CPlayerScript.h>

CElevatorAppear::CElevatorAppear()
	: CState(ELEVATORAPPEAR)
{
}

CElevatorAppear::~CElevatorAppear()
{
}

void CElevatorAppear::finaltick()
{
	if (!GetOwner()->Animator2D()->IsPlaying())
		ChangeState(L"Open");
}

void CElevatorAppear::Enter()
{
	GetOwner()->Animator2D()->Play(L"Open", false);
	GamePlayStatic::Play2DSound(L"sound\\obj\\elev\\GPE_Elevator_Open_Up_01.wav", 1, 0.3f);

	CPlayerMgr::GetPlayer()->Deactivate();
}

void CElevatorAppear::Exit()
{
}