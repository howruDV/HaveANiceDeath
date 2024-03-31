#include "pch.h"
#include "CPlayerJumpFalling.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMovement.h>
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
		pMovement->SetVelocity(vNewVeloc);
	}

	if ((KEY_TAP(KEY::D)) && KEY_NONE(KEY::A))
	{
		Vec3 vNewVeloc = pMovement->GetVelocity() + Vec3(fSpeedInAir, 0.f, 0.f);
		pMovement->SetVelocity(vNewVeloc);
	}

	// change state
	if (GetOwner()->Movement()->IsGround())
	{
		ChangeState(L"Jump_Landing");
	}
}

void CPlayerJumpFalling::Enter()
{
	m_PlayerMgr = CPlayerMgr::PlayerMgr();
	GetOwner()->Animator2D()->Play(L"Jump_Falling", false);
}

void CPlayerJumpFalling::Exit()
{
}