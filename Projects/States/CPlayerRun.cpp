#include "pch.h"
#include "CPlayerRun.h"
#include "CPlayerJumpLanding.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMovement.h>
#include <Engine/CTransform.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerRun::CPlayerRun()
	:CState(PLAYERRUN)
	, m_PlayerMgr(nullptr)
{
}

CPlayerRun::~CPlayerRun()
{
}

void CPlayerRun::finaltick()
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

	// change state
	if ((KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
		|| ((KEY_RELEASED(KEY::A) || KEY_NONE(KEY::A)) && (KEY_RELEASED(KEY::D) || KEY_NONE(KEY::D))))
	{
		ChangeState(L"Run_ToIdle");
	}

	if (KEY_TAP(KEY::SPACE))
	{
		// @TODO Space 시간 따라 높이 조절
		static float JumpDT = 0.f;
		ChangeState(L"Jump_Start");
	}
}

void CPlayerRun::Enter()
{
	// play anim
	m_PlayerMgr = CPlayerMgr::PlayerMgr();
	if (GetFSM()->GetPrevState()->GetStateType() == PLAYERJUMPLANDING)
	{
		GetOwner()->Animator2D()->Play(L"Jump_Landing", false);
		GetOwner()->Animator2D()->PushNextAnim(L"Idle_ToRun", false);
		GetOwner()->Animator2D()->PushNextAnim(L"Run", true);
	}
	else
		GetOwner()->Animator2D()->Play(L"Run", true);
}

void CPlayerRun::Exit()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	GetOwner()->Animator2D()->ClearNextAnim();
}