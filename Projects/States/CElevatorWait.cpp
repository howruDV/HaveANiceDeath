#include "pch.h"
#include "CElevatorWait.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>

#include <Scripts\CPlayerMgr.h>
#include <Scripts\CPlayerScript.h>

CElevatorWait::CElevatorWait()
	: CState(ELEVATORWAIT)
{
}

CElevatorWait::~CElevatorWait()
{
}

void CElevatorWait::finaltick()
{
	bool canClose = *((int*)GetBlackboardData(L"bCanClose"));

	if (canClose)
	{
		float fDist = (CPlayerMgr::GetPlayer()->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos()).Length();
		
		if (fDist <= 300.f)
		{
			if (KEY_TAP(KEY::F))
			{
				ChangeState(L"Close");
			}
		}
	}
}

void CElevatorWait::Enter()
{
	bool canClose = *((int*)GetBlackboardData(L"bCanClose"));

	if (canClose)
	{
		GetOwner()->Animator2D()->Play(L"Open", false);
	}
}

void CElevatorWait::Exit()
{
}
