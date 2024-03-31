#include "pch.h"
#include "CPlayerPowerUp.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CAnim.h>

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
	{
		// »ó½Â
		if (GetOwner()->Animator2D()->GetCurAnimFrmIdx() < 28)
		{
			float TargetHeight = 480.f / (28 / 30.f);
			Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
			vPos.y += TargetHeight * DT;

			GetOwner()->Transform()->SetRelativePos(vPos);
		}
		// ÇÏ°­
		else
		{

		}

		return;
	}

	ChangeState(L"Idle");
}

void CPlayerPowerUp::Enter()
{
	//CMovement* pMovement = GetOwner()->Movement();
	//Vec3 vJumpVeloc = Vec3(0, 2000.f, 0);
	//pMovement->SetVelocity(vJumpVeloc);
	GetOwner()->Movement()->SetGround(false);
	m_LandPos = GetOwner()->Transform()->GetRelativePos();

	GetOwner()->Animator2D()->Play(L"PowerUp", false);
}

void CPlayerPowerUp::Exit()
{
}