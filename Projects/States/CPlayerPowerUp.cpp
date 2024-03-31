#include "pch.h"
#include "CPlayerPowerUp.h"

CPlayerPowerUp::CPlayerPowerUp()
	: CState(PLAYERPOWERUP)
{
}

CPlayerPowerUp::~CPlayerPowerUp()
{
}

void CPlayerPowerUp::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	ChangeState(L"Idle");
}

void CPlayerPowerUp::Enter()
{
	CMovement* pMovement = GetOwner()->Movement();
	Vec3 vJumpVeloc = Vec3(0, 2000.f, 0);
	pMovement->SetGround(false);
	pMovement->SetVelocity(vJumpVeloc);

	GetOwner()->Animator2D()->Play(L"PowerUp", false);
}

void CPlayerPowerUp::Exit()
{
}