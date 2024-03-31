#include "pch.h"
#include "CPlayerJumpStart.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMovement.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerJumpStart::CPlayerJumpStart()
	: CState(PLAYERJUMPSTART)
{
}

CPlayerJumpStart::~CPlayerJumpStart()
{
}

void CPlayerJumpStart::finaltick()
{
	CMovement* pMovement = GetOwner()->Movement();
	float fSpeedInAir = *((float*)GetBlackboardData(L"fSpeedInAir"));

	// jump
	if ((KEY_TAP(KEY::A)) && KEY_NONE(KEY::D))
	{
		Vec3 vNewVeloc = pMovement->GetVelocity() + Vec3(-fSpeedInAir, 0.f, 0.f);
		pMovement->SetVelocity(vNewVeloc);
	}

	if ((KEY_TAP(KEY::D)) && KEY_NONE(KEY::A))
	{
		Vec3 vNewVeloc = pMovement->GetVelocity() + Vec3(fSpeedInAir, 0.f, 0.f);
		pMovement->SetVelocity(vNewVeloc);
	}

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	if (KEY_TAP(KEY::LSHIFT))
	{
		ChangeState(L"Dash");
		return;
	}

	// change state
	ChangeState(L"Jump_Falling");
}

void CPlayerJumpStart::Enter()
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

	// play jump
	GetOwner()->Animator2D()->Play(L"Jump_Start", false);
}

void CPlayerJumpStart::Exit()
{
}