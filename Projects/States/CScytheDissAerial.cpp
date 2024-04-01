#include "pch.h"
#include "CScytheDissAerial.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissAerial::CScytheDissAerial()
	: CState(SCYTHEDISSAERIAL)
{
}

CScytheDissAerial::~CScytheDissAerial()
{
}

void CScytheDissAerial::finaltick()
{
	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	if (GetOwner()->Movement()->IsGround())
		ChangeState(L"Idle");
	else
		ChangeState(L"Jump_Falling");
}

void CScytheDissAerial::Enter()
{
	if (KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D))
	{
		GetOwner()->Movement()->SetVelocity(Vec3());
		GetOwner()->Movement()->UseGravity(false);
	}

	GetOwner()->Animator2D()->Play(L"ScytheDiss_Aerial", false);
	PLAYERSCRIPT->DeactiveAerialAttack();
}

void CScytheDissAerial::Exit()
{
	GetOwner()->Movement()->UseGravity(true);
}
