#include "pch.h"
#include "CPlayerConcentrate.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerConcentrate::CPlayerConcentrate()
	: CState(PLAYERCONCENTRATE)
{
}

CPlayerConcentrate::~CPlayerConcentrate()
{
}

void CPlayerConcentrate::finaltick()
{	
	// change state
	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}

	if (KEY_RELEASED(KEY::LBTN) || KEY_NONE(KEY::LBTN))
		ChangeState(L"PowerUp");
}

void CPlayerConcentrate::Enter()
{
	GetOwner()->Animator2D()->Play(L"Concentrate", true);
}

void CPlayerConcentrate::Exit()
{
}