#include "pch.h"
#include "CPlayerRunToIdle.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerRunToIdle::CPlayerRunToIdle()
	: CState(PLAYERRUNTOIDLE)
{
}

CPlayerRunToIdle::~CPlayerRunToIdle()
{
}

void CPlayerRunToIdle::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		// change state: Turn
		// @TODO 지연확인
		if (((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::D))
		|| ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::A)))
		{
			if (PLAYERSCRIPT->WillDirChange())
				ChangeState(L"Run_UTurn");
		}

		return;
	}

	// change state
	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}

	ChangeState(L"Idle");
}

void CPlayerRunToIdle::Enter()
{
	// anim
	GetOwner()->Animator2D()->Play(L"Run_ToIdle", false);
}

void CPlayerRunToIdle::Exit()
{
}