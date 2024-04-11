#include "pch.h"
#include "CScytheDissComboD.h"

#include <Engine\CTransform.h>
#include <Engine\CCollider2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissComboD::CScytheDissComboD()
	: CState(SCYTHEDISSCOMBOD)
{
}

CScytheDissComboD::~CScytheDissComboD()
{
}


void CScytheDissComboD::finaltick()
{
	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if (CurFrame >= 15 && CurFrame <= 16)
	{
		Scale = Vec3(208, 214, 0);
		Offset = Vec3(104, 64, 0);
	}
	else if (CurFrame == 14)
	{
		Scale = Vec3(434, 214, 0);
		Offset = Vec3(194, 105, 0);
	}
	else if (CurFrame >= 10 && CurFrame <= 13)
	{
		Scale = Vec3(424, 242, 0);
		Offset = Vec3(-194, 121, 0);
	}
	else if (CurFrame == 9)
	{
		Scale = Vec3(582, 160, 0);
		Offset = Vec3(-116, -45, 0);
	}
	else if (CurFrame == 8)
	{
		Scale = Vec3(594, 160, 0);
		Offset = Vec3(128, -45, 0);
	}
	else if (CurFrame >= 4 && CurFrame <= 7)
	{
		Scale = Vec3(460, 140, 0);
		Offset = Vec3(230, 0, 0);
	}
	else if (CurFrame >= 2 && CurFrame <= 3)
	{
		Scale = Vec3(530, 240, 0);
		Offset = Vec3(157, 120, 0);
	}
	else if (CurFrame <= 1)
	{
		Scale = Vec3(448, 244, 0);
		Offset = Vec3(-35, 122, 0);
	}

	if (PLAYERSCRIPT->GetDir() == UNIT_DIRX::LEFT)
	{
		Scale.x *= -1.f;
		Offset.x *= -1.f;
	}

	m_pHitbox->Transform()->SetRelativeScale(Scale);
	m_pHitbox->Transform()->SetRelativePos(Offset);

	// change state - always
	if (KEY_TAP(KEY::SPACE) || KEY_PRESSED(KEY::SPACE))
	{
		ChangeState(L"Jump_Start");
	}

	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CScytheDissComboD::Enter()
{
	GetOwner()->Animator2D()->Play(L"ScytheDiss_ComboD", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	// hitbox
	m_pHitbox = GetOwner()->GetChildByName(L"ComboD_Hitbox");
	SetPlayerHitboxDamage(m_pHitbox, SCYTHE_DAMAGE_NAME::Air);
	m_pHitbox->Collider2D()->Activate();
}

void CScytheDissComboD::Exit()
{
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	PLAYERSCRIPT->DeactiveCombo();
	m_pHitbox->Collider2D()->Deactivate();
}