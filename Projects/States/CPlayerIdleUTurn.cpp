#include "pch.h"
#include "CPlayerIdleUTurn.h"

#include "Scripts/CPlayerMgr.h"
#include "Scripts/CPlayerScript.h"
#include "Scripts/CScytheScript.h"

CPlayerIdleUTurn::CPlayerIdleUTurn()
	: CState(PLAYERIDLEUTURN)
	, m_PlayerMgr(nullptr)
{
}

CPlayerIdleUTurn::~CPlayerIdleUTurn()
{
}


void CPlayerIdleUTurn::finaltick()
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

	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}

	if (KEY_TAP(KEY::SPACE) || KEY_PRESSED(KEY::SPACE))
	{
		ChangeState(L"Jump_Start");
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
	if ((KEY_TAP(KEY::A) && KEY_NONE(KEY::D)) || (KEY_TAP(KEY::D) && KEY_NONE(KEY::A)))
	{
		if (m_PlayerMgr->GetPlayerScript()->WillDirChange())
		{
			ChangeState(L"Idle_UTurn");
		}
		else
		{
			ChangeState(L"Idle_ToRun");
		}
	}

	if ((KEY_PRESSED(KEY::A) && KEY_NONE(KEY::D)) || (KEY_PRESSED(KEY::D) && KEY_NONE(KEY::A)))
	{
		ChangeState(L"Idle_ToRun");
	}

	if (((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)))
		|| ((KEY_NONE(KEY::A) || KEY_RELEASED(KEY::A)) && (KEY_NONE(KEY::D) || KEY_RELEASED(KEY::D))))
	{
		ChangeState(L"Idle");
	}
}

void CPlayerIdleUTurn::Enter()
{
	m_PlayerMgr = CPlayerMgr::PlayerMgr();
	m_PlayerMgr->GetPlayerScript()->SetDirLock(true);
	GetOwner()->Movement()->SetVelocity(Vec3());

	// play anim
	GetOwner()->Animator2D()->Play(L"Idle_UTurn", false);
}

void CPlayerIdleUTurn::Exit()
{
	m_PlayerMgr->GetPlayerScript()->SetDirLock(false);
	GetOwner()->Movement()->SetVelocity(Vec3());
}