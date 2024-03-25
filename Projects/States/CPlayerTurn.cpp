#include "pch.h"
#include "CPlayerTurn.h"
#include "CPlayerRun.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMovement.h>

#include "Scripts/CPlayerMgr.h"
#include "Scripts/CPlayerScript.h"

CPlayerTurn::CPlayerTurn()
	: CState(PLAYERTURN)
	, m_PlayerMgr(nullptr)
{
}

CPlayerTurn::~CPlayerTurn()
{
}


void CPlayerTurn::finaltick()
{
	// turn 중이면 state 변경 보류
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	// change state

	if (KEY_TAP(KEY::A) && KEY_NONE(KEY::D))
	{
		if (not m_PlayerMgr->GetPlayerScript()->GetLookLeft())
		{
			ChangeState(L"Turn");
		}
		else
		{
			GetOwner()->Animator2D()->Play(L"Idle_ToRun", false);
			ChangeState(L"Run");
		}
	}

	if (KEY_TAP(KEY::D) && KEY_NONE(KEY::A))
	{
		if (m_PlayerMgr->GetPlayerScript()->GetLookLeft())
		{
			ChangeState(L"Turn");
		}
		else
		{
			GetOwner()->Animator2D()->Play(L"Idle_ToRun", false);
			ChangeState(L"Run");
		}
	}

	if (KEY_PRESSED(KEY::A) && KEY_NONE(KEY::D))
	{
		GetOwner()->Animator2D()->Play(L"Idle_ToRun", false);
		ChangeState(L"Run");
	}

	if (KEY_PRESSED(KEY::D) && KEY_NONE(KEY::A))
	{
		GetOwner()->Animator2D()->Play(L"Idle_ToRun", false);
		ChangeState(L"Run");
	}

	if (((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)))
		|| ((KEY_NONE(KEY::A) || KEY_RELEASED(KEY::A)) && (KEY_NONE(KEY::D) || KEY_RELEASED(KEY::D))))
	{
		ChangeState(L"Idle");
	}
}

void CPlayerTurn::Enter()
{
	m_PlayerMgr = CPlayerMgr::PlayerMgr();

	if (dynamic_cast<CPlayerRun*>(GetFSM()->GetPrevState()))
		GetOwner()->Animator2D()->Play(L"Run_UTurn", false);
	else
		GetOwner()->Animator2D()->Play(L"Idle_UTurn", false);
}

void CPlayerTurn::Exit()
{
	if (m_PlayerMgr->GetPlayerScript()->GetLookLeft())
		m_PlayerMgr->GetPlayerScript()->SetLookLeft(false);
	else
		m_PlayerMgr->GetPlayerScript()->SetLookLeft(true);

	GetOwner()->Animator2D()->ClearNextAnim();
}