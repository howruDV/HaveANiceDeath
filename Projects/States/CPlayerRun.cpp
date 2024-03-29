#include "pch.h"
#include "CPlayerRun.h"
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
	// change anim
	if (GetOwner()->Animator2D()->GetCurAnimName() != L"Run")
		return;

	// Run
	float fSpeed = *((float*)GetBlackboardData(L"fSpeed"));

	if (KEY_PRESSED(KEY::A) && KEY_NONE(KEY::D))
	{
		Vec3 vSpeed = Vec3(-fSpeed, 0, 0);
		GetOwner()->Movement()->AddForce(vSpeed);
		m_PlayerMgr->GetPlayerScript()->SetLookLeft(true);
	}
	if (KEY_PRESSED(KEY::D) && KEY_NONE(KEY::A))
	{
		Vec3 vSpeed = Vec3(fSpeed, 0, 0);
		GetOwner()->Movement()->AddForce(vSpeed);
		m_PlayerMgr->GetPlayerScript()->SetLookLeft(false);
	}
	
	// Change State
	if ((KEY_RELEASED(KEY::A) && (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)))
		|| (KEY_RELEASED(KEY::D) && (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))))
	{
		// @TODO : Run Turn 지연확인
		ChangeState(L"Turn");
	}

	if ((KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
		|| ((KEY_RELEASED(KEY::A) || KEY_NONE(KEY::A)) && (KEY_RELEASED(KEY::D) || KEY_NONE(KEY::D))))
	{
		GetOwner()->Animator2D()->Play(L"Run_ToIdle", false);
		ChangeState(L"Idle");
	}

	if (KEY_TAP(KEY::SPACE) || KEY_PRESSED(KEY::SPACE))
	{
		static float JumpDT = 0.f;
		ChangeState(L"Jump");
		// @TODO Space 시간 따라 높이 조절
	}
}

void CPlayerRun::Enter()
{
	m_PlayerMgr = CPlayerMgr::PlayerMgr();
	GetOwner()->Animator2D()->PushNextAnim(L"Run", true);
}

void CPlayerRun::Exit()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	GetOwner()->Animator2D()->ClearNextAnim();
}