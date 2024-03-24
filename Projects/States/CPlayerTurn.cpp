#include "pch.h"
#include "CPlayerTurn.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>

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
	if (GetOwner()->Animator2D()->GetCurAnimName() == L"Idle_UTurn" && GetOwner()->Animator2D()->IsPlaying())
		return;

	// change state
	if (KEY_NONE(KEY::A) || KEY_NONE(KEY::D))
	{
		ChangeState(L"Idle");
	}

	// -----------idle 복사-----------
	if (KEY_TAP(KEY::A) && KEY_NONE(KEY::D))
	{
		if (not m_PlayerMgr->GetPlayerScript()->GetLookLeft())
		{
			ChangeState(L"Turn");
		}
	}

	if (KEY_TAP(KEY::D) && KEY_NONE(KEY::A))
	{
		if (m_PlayerMgr->GetPlayerScript()->GetLookLeft())
		{
			ChangeState(L"Turn");
		}
	}

	if ((KEY_PRESSED(KEY::A) && KEY_NONE(KEY::D)) || (KEY_PRESSED(KEY::D) && KEY_NONE(KEY::A)))
	{
		GetOwner()->Animator2D()->Play(L"Idle_ToRun", false);
		ChangeState(L"Run");
	}
}

void CPlayerTurn::Enter()
{
	m_PlayerMgr = CPlayerMgr::PlayerMgr();
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