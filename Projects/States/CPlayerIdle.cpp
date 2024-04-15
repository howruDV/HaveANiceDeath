#include "pch.h"
#include "CPlayerIdle.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerIdle::CPlayerIdle()
	: CState(PLAYERIDLE)
{
}

CPlayerIdle::~CPlayerIdle()
{
}

void CPlayerIdle::finaltick()
{
	// change state
	if (((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::D))
	|| ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::A)))
	{
		if (PLAYERSCRIPT->WillDirChange())
		{
			ChangeState(L"Idle_UTurn");
		}
		else
		{
			ChangeState(L"Idle_ToRun");
		}
	}

	if (((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_RELEASED(KEY::D))
		|| ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_RELEASED(KEY::A)))
		if (PLAYERSCRIPT->WillDirChange())
			ChangeState(L"Idle_UTurn");

	if (KEY_TAP(KEY::LBTN) && (KEY_NONE(KEY::S) || KEY_RELEASED(KEY::S)))
	{
		int nextCombo = PLAYERSCRIPT->GetNextComboIdx();
		wstring strCurScytheName = PLAYERSCRIPT->GetScytheName();

		if (nextCombo == 0)
			ChangeState(strCurScytheName + L"_ComboA");
		else if (nextCombo == 1)
			ChangeState(strCurScytheName + L"_ComboB");
		else if (nextCombo == 2)
			ChangeState(strCurScytheName + L"_ComboC");
		else if (nextCombo == 3)
			ChangeState(strCurScytheName + L"_ComboD");
	}
	else if (KEY_PRESSED(KEY::LBTN))
	{
		ChangeState(L"Concentrate_Start");
	}

	if (KEY_TAP(KEY::LBTN) && (KEY_PRESSED(KEY::S) || KEY_TAP(KEY::S)))
	{
		wstring strCurScytheName = PLAYERSCRIPT->GetScytheName();
		ChangeState(strCurScytheName + L"_Special");
	}

	if (KEY_TAP(KEY::SPACE) || KEY_PRESSED(KEY::SPACE))
	{
		ChangeState(L"Jump_Start");
	}

	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
	}

	if ((KEY_PRESSED(KEY::W) || KEY_TAP(KEY::W)) && KEY_TAP(KEY::LBTN))
	{
		wstring strCurScytheName = PLAYERSCRIPT->GetScytheName();
		ChangeState(strCurScytheName +L"_Up");
	}

	if ((KEY_PRESSED(KEY::Q) || KEY_TAP(KEY::Q)) && KEY_TAP(KEY::LBTN))
	{
		if (CPlayerMgr::GetPlayerScript()->CanRest())
		{
			wstring strCurScytheName = PLAYERSCRIPT->GetScytheName();
			ChangeState(strCurScytheName +L"_Rest");
		}
	}
}

void CPlayerIdle::Enter()
{
	// setting
	GetOwner()->Movement()->SetVelocity(Vec3());

	// anim
	GetOwner()->Animator2D()->Play(L"Idle", true);
}

void CPlayerIdle::Exit()
{
}