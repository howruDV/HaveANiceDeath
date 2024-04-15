#include "pch.h"
#include "CPlayerRunToIdle.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CInvenMgr.h>

CPlayerRunToIdle::CPlayerRunToIdle()
	: CState(PLAYERRUNTOIDLE)
{
}

CPlayerRunToIdle::~CPlayerRunToIdle()
{
}

void CPlayerRunToIdle::finaltick()
{
	if (KEY_TAP(KEY::SPACE) || KEY_PRESSED(KEY::SPACE))
	{
		ChangeState(L"Jump_Start");
		return;
	}

	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}

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
	}
	else if (KEY_PRESSED(KEY::LBTN))
	{
		ChangeState(L"Concentrate_Start");
	}

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		// change state: Turn
		if (((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::D))
		|| ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::A)))
		{
			if (PLAYERSCRIPT->WillDirChange())
				ChangeState(L"Run_UTurn");
		}

		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CPlayerRunToIdle::Enter()
{
	// anim
	GetOwner()->Animator2D()->Play(L"Run_ToIdle", false);
}

void CPlayerRunToIdle::Exit()
{
}