#include "pch.h"
#include "CPlayerRunUTurn.h"
#include "CPlayerRun.h"
#include "CPlayerRunToIdle.h"

#include "Scripts/CPlayerMgr.h"
#include "Scripts/CPlayerScript.h"

CPlayerRunUTurn::CPlayerRunUTurn()
	: CState(PLAYERRUNUTURN)
	, m_PlayerMgr(nullptr)
{
}

CPlayerRunUTurn::~CPlayerRunUTurn()
{
}

void CPlayerRunUTurn::finaltick()
{
	// Run
	float fSpeed = *((float*)GetBlackboardData(L"fSpeed"));

	if (KEY_PRESSED(KEY::A) && KEY_NONE(KEY::D))
	{
		Vec3 vSpeed = Vec3(-fSpeed, 0, 0);
		GetOwner()->Movement()->AddForce(vSpeed);
	}
	if (KEY_PRESSED(KEY::D) && KEY_NONE(KEY::A))
	{
		Vec3 vSpeed = Vec3(fSpeed, 0, 0);
		GetOwner()->Movement()->AddForce(vSpeed);
	}
	if ((KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
		|| ((KEY_RELEASED(KEY::A) || KEY_NONE(KEY::A)) && (KEY_RELEASED(KEY::D) || KEY_NONE(KEY::D))))
	{
		GetOwner()->Movement()->SetVelocity(Vec3());
	}

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
		wstring strCurScytheName = PLAYERSCRIPT->GetScytheName();

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

	// turn 중이면 state 변경 보류
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	// change state
	if (KEY_TAP(KEY::A) && KEY_NONE(KEY::D))
	{
		if (m_PlayerMgr->GetPlayerScript()->WillDirChange())
		{
			ChangeState(L"Run_UTurn");
		}
		else
		{
			ChangeState(L"Run");
		}
	}

	if (KEY_TAP(KEY::D) && KEY_NONE(KEY::A))
	{
		if (m_PlayerMgr->GetPlayerScript()->WillDirChange())
		{
			ChangeState(L"Run_UTurn");
		}
		else
		{
			ChangeState(L"Run");
		}
	}

	if (KEY_PRESSED(KEY::A) && KEY_NONE(KEY::D))
	{
		ChangeState(L"Run");
	}

	if (KEY_PRESSED(KEY::D) && KEY_NONE(KEY::A))
	{
		ChangeState(L"Run");
	}

	if (((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)))
		|| ((KEY_NONE(KEY::A) || KEY_RELEASED(KEY::A)) && (KEY_NONE(KEY::D) || KEY_RELEASED(KEY::D))))
	{
		ChangeState(L"Idle");
	}
}

void CPlayerRunUTurn::Enter()
{
	m_PlayerMgr = CPlayerMgr::PlayerMgr();
	m_PlayerMgr->GetPlayerScript()->SetDirLock(true);

	// play anim
	GetOwner()->Animator2D()->Play(L"Run_UTurn", false);
}

void CPlayerRunUTurn::Exit()
{
	m_PlayerMgr->GetPlayerScript()->SetDirLock(false);
	GetOwner()->Movement()->SetVelocity(Vec3());
}