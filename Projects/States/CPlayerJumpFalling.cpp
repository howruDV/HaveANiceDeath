#include "pch.h"
#include "CPlayerJumpFalling.h"

#include <Engine/CTransform.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerJumpFalling::CPlayerJumpFalling()
	: CState(PLAYERJUMPFALLING)
{
}

CPlayerJumpFalling::~CPlayerJumpFalling()
{
}

void CPlayerJumpFalling::finaltick()
{
	CMovement* pMovement = GetOwner()->Movement();
	float fSpeedInAir = *((float*)GetBlackboardData(L"fSpeedInAir"));

	// falling
	if ((KEY_TAP(KEY::A)) && KEY_NONE(KEY::D))
	{
		Vec3 vNewVeloc = pMovement->GetVelocity() + Vec3(-fSpeedInAir, 0.f, 0.f);
		pMovement->AddForce(vNewVeloc);
	}

	if ((KEY_TAP(KEY::D)) && KEY_NONE(KEY::A))
	{
		Vec3 vNewVeloc = pMovement->GetVelocity() + Vec3(fSpeedInAir, 0.f, 0.f);
		pMovement->AddForce(vNewVeloc);
	}

	// change state
	if (GetOwner()->Movement()->IsGround())
	{
		ChangeState(L"Jump_Landing");
	}

	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
	}
}

void CPlayerJumpFalling::Enter()
{
	GetOwner()->Animator2D()->Play(L"Jump_Falling", false);
}

void CPlayerJumpFalling::Exit()
{
}