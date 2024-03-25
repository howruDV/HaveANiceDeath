#include "pch.h"
#include "CPlayerJump.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMovement.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerJump::CPlayerJump()
	: CState(PLAYERJUMP)
	, m_PlayerMgr(nullptr)
{
}

CPlayerJump::~CPlayerJump()
{
}

void CPlayerJump::finaltick()
{
	// change anim
	if (GetOwner()->Animator2D()->GetCurAnimName() != L"Jump_Falling")
		return;

	CMovement* pMovement = GetOwner()->Movement();
	Vec3 vVeloc = pMovement->GetVelocity();
	float fSpeedInAir = *((float*)GetBlackboardData(L"fSpeedInAir"));

	// falling
	if ((KEY_TAP(KEY::A)) && KEY_NONE(KEY::D))
	{
		if (not m_PlayerMgr->GetPlayerScript()->GetLookLeft())
			m_PlayerMgr->GetPlayerScript()->SetLookLeft(true);

		Vec3 vNewVeloc = pMovement->GetVelocity() + Vec3(-fSpeedInAir, 0.f, 0.f);
		pMovement->SetVelocity(vNewVeloc);
	}

	if ((KEY_TAP(KEY::D)) && KEY_NONE(KEY::A))
	{
		if (m_PlayerMgr->GetPlayerScript()->GetLookLeft())
			m_PlayerMgr->GetPlayerScript()->SetLookLeft(false);

		Vec3 vNewVeloc = pMovement->GetVelocity() + Vec3(fSpeedInAir, 0.f, 0.f);
		pMovement->SetVelocity(vNewVeloc);
	}

	// change state
	if (GetOwner()->Movement()->IsGround())
	{
		ChangeState(L"Idle");
		GetOwner()->Animator2D()->Play(L"Jump_Landing", false);
	}
}

void CPlayerJump::Enter()
{
	m_PlayerMgr = CPlayerMgr::PlayerMgr();
	CMovement* pMovement = GetOwner()->Movement();
	float fJumpVeloc = *((float*)GetBlackboardData(L"fJumpVelocMax"));
	float fSpeed = *((float*)GetBlackboardData(L"fSpeed"));
	Vec3 vJumpVeloc = Vec3(0, fJumpVeloc, 0);

	pMovement->SetGround(false);

	if ((KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::D))
		vJumpVeloc.x = -fSpeed;
	if ((KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::A))
		vJumpVeloc.x = fSpeed;
	
	pMovement->SetVelocity(vJumpVeloc);

	GetOwner()->Animator2D()->Play(L"Jump_Start", false);
	GetOwner()->Animator2D()->Play(L"Jump_Falling", false);
}

void CPlayerJump::Exit()
{
	GetOwner()->Animator2D()->ClearNextAnim();
	GetOwner()->Movement()->SetVelocity(Vec3());
}
