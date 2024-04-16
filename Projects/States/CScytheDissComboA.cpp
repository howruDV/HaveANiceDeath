#include "pch.h"
#include "CScytheDissComboA.h"

#include <Engine\CTransform.h>
#include <Engine\CCollider2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissComboA::CScytheDissComboA()
	: CState(SCYTHEDISSCOMBOA)
	, m_bCombo(false)
{
}

CScytheDissComboA::~CScytheDissComboA()
{
}

void CScytheDissComboA::finaltick()
{
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

	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if (CurFrame >= 8)
	{
		Scale = Vec3(235, 210, 0);
		Offset = Vec3();
	}
	else if (CurFrame >= 6 && CurFrame <= 7)
	{
		Scale = Vec3(350, 200, 0);
		Offset = Vec3(100, 0, 0);
	}
	else if (CurFrame <= 5)
	{
		Scale = Vec3(530, 165, 0);
		Offset = Vec3(260, 0, 0);
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
	if (m_bCombo)
	{
		ChangeState(L"ScytheDiss_ComboB");
		return;
	}

	ChangeState(L"Idle");
}

void CScytheDissComboA::Enter()
{
	GamePlayStatic::Play2DSound(L"sound\\scythe\\diss\\PC_Atk_Flail_Combo1_01.wav", 1, 0.25f);
	GetOwner()->Animator2D()->Play(L"ScytheDiss_ComboA", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	// hitbox
	m_pHitbox = GetOwner()->GetChildByName(L"ComboA_Hitbox");
	SetPlayerHitboxDamage(m_pHitbox, SCYTHE_DAMAGE_NAME::Air);
	m_pHitbox->Collider2D()->Activate();
}

void CScytheDissComboA::Exit()
{
	PLAYERSCRIPT->StartCombo(0);
	m_bCombo = false;
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_pHitbox->Collider2D()->Deactivate();
}
