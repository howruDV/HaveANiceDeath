#include "pch.h"
#include "CPlayerJumpLanding.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMovement.h>
#include <Engine/CTransform.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerJumpLanding::CPlayerJumpLanding()
	: CState(PLAYERJUMPLANDING)
{
}

CPlayerJumpLanding::~CPlayerJumpLanding()
{
}

void CPlayerJumpLanding::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	ChangeState(L"Idle");
}

void CPlayerJumpLanding::Enter()
{
	m_PlayerMgr = CPlayerMgr::PlayerMgr();

	// set velocity
	Vec3 vVeloc = GetOwner()->Movement()->GetVelocity();
	vVeloc.x = 0.f;
	GetOwner()->Movement()->SetVelocity(vVeloc);

	// change anim
	GetOwner()->Animator2D()->Play(L"Jump_Landing", false);

	// change state
	if ((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::D))
	{
		ChangeState(L"Run");
	}

	if ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::A))
	{
		ChangeState(L"Run");
	}
}

void CPlayerJumpLanding::Exit()
{
}
