#include "pch.h"
#include "CPlayerConcentrate_Start.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerConcentrate_Start::CPlayerConcentrate_Start()
	: CState(PLAYERCONCENTRATE_START)
{
}

CPlayerConcentrate_Start::~CPlayerConcentrate_Start()
{
}

void CPlayerConcentrate_Start::finaltick()
{
	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}
	
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		if (KEY_RELEASED(KEY::LBTN) || KEY_NONE(KEY::LBTN))
			ChangeState(L"Idle");

		return;
	}

	// change state
	ChangeState(L"Concentrate");
}

void CPlayerConcentrate_Start::Enter()
{
	GetOwner()->Animator2D()->Play(L"Concentrate_Start", false);
}

void CPlayerConcentrate_Start::Exit()
{
}