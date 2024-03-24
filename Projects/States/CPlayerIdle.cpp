#include "pch.h"
#include "CPlayerIdle.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>

CPlayerIdle::CPlayerIdle()
	:CState(PLAYERIDLE)
{
}

CPlayerIdle::~CPlayerIdle()
{
}

void CPlayerIdle::finaltick()
{
	if	((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::D)
		|| ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::A)))
	{
		GetOwner()->Animator2D()->Play(L"Idle_ToRun", false);
		ChangeState(L"Run");
	}
}

void CPlayerIdle::Enter()
{
	GetOwner()->Animator2D()->PushNextAnim(L"Idle");
}

void CPlayerIdle::Exit()
{
	GetOwner()->Animator2D()->ClearNextAnim();
}