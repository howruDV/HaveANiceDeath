#include "pch.h"
#include "CPlayerIdle.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMovement.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerIdle::CPlayerIdle()
	: CState(PLAYERIDLE)
	, m_PlayerMgr(nullptr)
{
}

CPlayerIdle::~CPlayerIdle()
{
}

void CPlayerIdle::finaltick()
{
	// change anim
	if (GetOwner()->Animator2D()->GetCurAnimName() != L"Idle")
		return;

	// change state
	if ((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::D))
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

	if ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::A))
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

	if (KEY_TAP(KEY::SPACE) || KEY_PRESSED(KEY::SPACE))
	{
		static float JumpDT = 0.f;
		ChangeState(L"Jump");
		// @TODO Space 시간 따라 높이 조절
	}
}

void CPlayerIdle::Enter()
{
	m_PlayerMgr = CPlayerMgr::PlayerMgr();
	GetOwner()->Animator2D()->PushNextAnim(L"Idle", true);
}

void CPlayerIdle::Exit()
{
	GetOwner()->Animator2D()->ClearNextAnim();
}