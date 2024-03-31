#include "pch.h"
#include "CPlayerRunToIdle.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerRunToIdle::CPlayerRunToIdle()
	: CState(PLAYERRUNTOIDLE)
{
}

CPlayerRunToIdle::~CPlayerRunToIdle()
{
}

void CPlayerRunToIdle::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		// change state: Turn
		// @TODO ����Ȯ��
		if (((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::D))
		|| ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::A)))
		{
			if (m_PlayerMgr->GetPlayerScript()->WillDirChange())
				ChangeState(L"Run_UTurn");
		}

		return;
	}

	// change state
	if (KEY_TAP(KEY::LSHIFT))
	{
		ChangeState(L"Dash");
		return;
	}

	ChangeState(L"Idle");
}

void CPlayerRunToIdle::Enter()
{
	// setting
	m_PlayerMgr = CPlayerMgr::PlayerMgr();

	// anim
	GetOwner()->Animator2D()->Play(L"Run_ToIdle", false);
}

void CPlayerRunToIdle::Exit()
{
}