#include "pch.h"
#include "CScytheDissComboB.h"

#include <Engine\CTransform.h>
#include <Engine\CCollider2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissComboB::CScytheDissComboB()
	: CState(SCYTHEDISSCOMBOB)
	, m_bCombo(false)
{
}

CScytheDissComboB::~CScytheDissComboB()
{
}

void CScytheDissComboB::finaltick()
{
	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if (CurFrame >= 16 && CurFrame <= 18)
	{
		Scale = Vec3(230, 200, 0);
		Offset = Vec3(115, 0, 0);
	}
	else if (CurFrame >= 14 && CurFrame <= 15)
	{
		Scale = Vec3(310, 260, 0);
		Offset = Vec3(0, 130, 0);
	}
	else if (CurFrame >= 10 && CurFrame <= 13)
	{
		Scale = Vec3(556, 240, 0);
		Offset = Vec3(140, 0, 0);
	}
	else if (CurFrame >= 5 && CurFrame <= 9)
	{
		Scale = Vec3(690, 288, 0);
		Offset = Vec3(220, 0, 0);
	}
	else if (CurFrame <= 4)
	{
		Scale = Vec3(300, 270, 0);
		Offset = Vec3();
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

	if (KEY_TAP(KEY::LBTN))
		m_bCombo = true;

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	if (m_bCombo)
	{
		ChangeState(L"ScytheDiss_ComboC");
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CScytheDissComboB::Enter()
{
	GamePlayStatic::Play2DSound(L"sound\\scythe\\diss\\PC_Atk_Flail_Combo2_01.wav", 1, 0.25f);
	GetOwner()->Animator2D()->Play(L"ScytheDiss_ComboB", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	// hitbox
	m_pHitbox = GetOwner()->GetChildByName(L"ComboB_Hitbox");
	SetPlayerHitboxDamage(m_pHitbox, SCYTHE_DAMAGE_NAME::Air);
	m_pHitbox->Collider2D()->Activate();
}

void CScytheDissComboB::Exit()
{
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	PLAYERSCRIPT->StartCombo(1);
	m_bCombo = false;
		m_pHitbox->Collider2D()->Deactivate();
}