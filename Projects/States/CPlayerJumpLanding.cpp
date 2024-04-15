#include "pch.h"
#include "CPlayerJumpLanding.h"

#include <Engine/CTransform.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CInvenMgr.h>

CPlayerJumpLanding::CPlayerJumpLanding()
	: CState(PLAYERJUMPLANDING)
{
}

CPlayerJumpLanding::~CPlayerJumpLanding()
{
}

void CPlayerJumpLanding::finaltick()
{
	if (KEY_TAP(KEY::LBTN))
	{
		int nextCombo = PLAYERSCRIPT->GetNextComboIdx();
		wstring strCurScytheName = INVENTORY->GetScytheName();

		if (nextCombo == 0)
			ChangeState(strCurScytheName + L"_ComboA");
		else if (nextCombo == 1)
			ChangeState(strCurScytheName + L"_ComboB");
		else if (nextCombo == 2)
			ChangeState(strCurScytheName + L"_ComboC");
		else if (nextCombo == 3)
			ChangeState(strCurScytheName + L"_ComboD");

		return;
	}
	else if (KEY_PRESSED(KEY::LBTN))
	{
		ChangeState(L"Concentrate_Start");
		return;
	}

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	ChangeState(L"Idle");
}

void CPlayerJumpLanding::Enter()
{
	//m_PlayerMgr = CPlayerMgr::PlayerMgr();

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

	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
	}
}

void CPlayerJumpLanding::Exit()
{
}
