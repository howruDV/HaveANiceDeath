#include "pch.h"
#include "func.h"
#include "CScytheDissAerial.h"

#include <Engine\CTransform.h>
#include <Engine\CCollider2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CHitboxScript.h>

CScytheDissAerial::CScytheDissAerial()
	: CState(SCYTHEDISSAERIAL)
{
}

CScytheDissAerial::~CScytheDissAerial()
{
}

void CScytheDissAerial::finaltick()
{
	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}

	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if (CurFrame >= 9)
	{
		Scale = Vec3(300, 325, 0);
		Offset = Vec3();
	}
	else if (CurFrame >= 7 && CurFrame <= 8)
	{
		Scale = Vec3(500, 420, 0);
		Offset = Vec3(-130, 0, 0);
	}
	else if (CurFrame >= 2 && CurFrame <= 6)
	{
		Scale = Vec3(735, 330, 0);
		Offset = Vec3(277, 0, 0);
	}
	else if (CurFrame <= 1)
	{
		Scale = Vec3(320, 210, 0);
		Offset = Vec3();
	}

	if (PLAYERSCRIPT->GetDir() == UNIT_DIRX::LEFT)
	{
		Scale.x *= -1.f;
		Offset.x *= -1.f;
	}

	m_pHitbox->Transform()->SetRelativeScale(Scale);
	m_pHitbox->Transform()->SetRelativePos(Offset);

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	if (GetOwner()->Movement()->IsGround())
		ChangeState(L"Idle");
	else
		ChangeState(L"Jump_Falling");
}

void CScytheDissAerial::Enter()
{
	if (KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D))
	{
		GetOwner()->Movement()->SetVelocity(Vec3());
		GetOwner()->Movement()->UseGravity(false);
	}

	GetOwner()->Animator2D()->Play(L"ScytheDiss_Aerial", false);

	PLAYERSCRIPT->DeactiveAerialAttack();
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	// hitbox
	m_pHitbox = GetOwner()->GetChildByName(L"Hitbox");
	SetPlayerHitboxDamage(m_pHitbox, SCYTHE_DAMAGE_NAME::Air);
	m_pHitbox->Collider2D()->Activate();
}

void CScytheDissAerial::Exit()
{
	GetOwner()->Movement()->UseGravity(true);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_pHitbox->Collider2D()->Deactivate();
}