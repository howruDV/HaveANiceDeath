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
	// change state
	if (((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::D))
	|| ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::A)))
	{
		if (m_PlayerMgr->GetPlayerScript()->IsDirChange())
		{
			ChangeState(L"Idle_UTurn");
		}
		else
		{
			ChangeState(L"Idle_ToRun");
		}
	}

	if (KEY_TAP(KEY::SPACE) || KEY_PRESSED(KEY::SPACE))
	{
		static float JumpDT = 0.f;
		ChangeState(L"Jump_Start");
		// @TODO Space 시간 따라 높이 조절
	}
}

void CPlayerIdle::Enter()
{
	// setting
	GetOwner()->Movement()->SetVelocity(Vec3());
	m_PlayerMgr = CPlayerMgr::PlayerMgr();

	// anim
	GetOwner()->Animator2D()->Play(L"Idle", true);
}

void CPlayerIdle::Exit()
{
}