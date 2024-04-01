#include "pch.h"
#include "CPlayerJumpStart.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CScytheScript.h>

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

	if (KEY_TAP(KEY::LBTN))
	{
		int nextCombo = PLAYERSCRIPT->GetNextComboIdx();
		wstring strCurScytheName = PLAYERSCRIPT->GetScythe()->GetName();

		if (PLAYERSCRIPT->CanAerialAttack())
			ChangeState(strCurScytheName + L"_Aerial");
	}

	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}

	if ((KEY_PRESSED(KEY::S) || KEY_TAP(KEY::S)) && KEY_TAP(KEY::LBTN))
	{
		wstring strCurScytheName = PLAYERSCRIPT->GetScythe()->GetName();
		ChangeState(strCurScytheName + L"_Crush");
		return;
	}

	if ((KEY_PRESSED(KEY::W) || KEY_TAP(KEY::W)) && KEY_TAP(KEY::LBTN))
	{
		wstring strCurScytheName = PLAYERSCRIPT->GetScythe()->GetName();
		ChangeState(strCurScytheName + L"_Up");
		return;
	}

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
		return;


	// change state
	ChangeState(L"Jump_Falling");
}

void CPlayerJumpStart::Enter()
{
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