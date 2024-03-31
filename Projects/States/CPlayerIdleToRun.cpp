#include "pch.h"
#include "CPlayerIdleToRun.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CMovement.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>

CPlayerIdleToRun::CPlayerIdleToRun()
	: CState(PLAYERIDLETORUN)
{
}

CPlayerIdleToRun::~CPlayerIdleToRun()
{
}

void CPlayerIdleToRun::finaltick()
{
	// Run
	float fSpeed = *((float*)GetBlackboardData(L"fSpeed"));

	if (KEY_PRESSED(KEY::A) && KEY_NONE(KEY::D))
	{
		Vec3 vSpeed = Vec3(-fSpeed, 0, 0);
		GetOwner()->Movement()->AddForce(vSpeed);
	}
	if (KEY_PRESSED(KEY::D) && KEY_NONE(KEY::A))
	{
		Vec3 vSpeed = Vec3(fSpeed, 0, 0);
		GetOwner()->Movement()->AddForce(vSpeed);
	}
	if ((KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
		|| ((KEY_RELEASED(KEY::A) || KEY_NONE(KEY::A)) && (KEY_RELEASED(KEY::D) || KEY_NONE(KEY::D))))
	{
		GetOwner()->Movement()->SetVelocity(Vec3());
	}

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	// change state
	ChangeState(L"Run");
}

void CPlayerIdleToRun::Enter()
{
	GetOwner()->Animator2D()->Play(L"Idle_ToRun", false);
}

void CPlayerIdleToRun::Exit()
{
}