#include "pch.h"
#include "CPlayerDash.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerDash::CPlayerDash()
	: CState(PLAYERDASH)
	, m_PlayerMgr(nullptr)
{
}

CPlayerDash::~CPlayerDash()
{
}

void CPlayerDash::finaltick()
{	
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	if ((KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
		|| ((KEY_RELEASED(KEY::A) || KEY_NONE(KEY::A)) && (KEY_RELEASED(KEY::D) || KEY_NONE(KEY::D))))
	{
		ChangeState(L"Idle");
	}

	// change state
	if (GetFSM()->GetPrevState()->GetStateType() == STATE_TYPE::PLAYERJUMPSTART || GetFSM()->GetPrevState()->GetStateType() == STATE_TYPE::PLAYERJUMPFALLING)
	{
		ChangeState(L"Jump_Falling");
		return;
	}
	
	if (((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && KEY_NONE(KEY::D))
		|| ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && KEY_NONE(KEY::A)))
	{
		ChangeState(L"Run");
	}
}

void CPlayerDash::Enter()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	m_PlayerMgr = CPlayerMgr::PlayerMgr();

	// set speed
	Vec3 vSpeed = Vec3();
	vSpeed.x = *((float*)GetBlackboardData(L"fSpeedDash"));
	if (m_PlayerMgr->GetPlayerScript()->GetDir() == UNIT_DIRX::LEFT)
		vSpeed.x *= -1;
	GetOwner()->Movement()->SetVelocity(vSpeed);

	// anim
	GetOwner()->Animator2D()->Play(L"Dash", true);
}

void CPlayerDash::Exit()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
}

