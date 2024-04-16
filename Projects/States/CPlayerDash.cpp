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
	if (!GetOwner()->Movement()->IsGround())
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

	// set speed
	Vec3 vSpeed = Vec3();
	vSpeed.x = *((float*)GetBlackboardData(L"fSpeedDash"));
	if (PLAYERSCRIPT->GetDir() == UNIT_DIRX::LEFT)
		vSpeed.x *= -1;
	GetOwner()->Movement()->UseMaxSpeed(false);
	GetOwner()->Movement()->SetVelocity(vSpeed);

	// anim
	GamePlayStatic::Play2DSound(L"sound\\player\\PC_Nav_Dash_Lgt_01.wav", 1, 0.25f);
	GetOwner()->Animator2D()->Play(L"Dash", true);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CPlayerDash::Exit()
{
	GetOwner()->Movement()->UseMaxSpeed(true);
	GetOwner()->Movement()->SetVelocity(Vec3());
	PLAYERSCRIPT->StartDashCoolTime();
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}