#include "pch.h"
#include "CScytheDissComboC.h"

#include <Engine\CTransform.h>
#include <Engine\CCollider2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts\CGameMgr.h>
#include <Scripts\CCamCtrlScript.h>

CScytheDissComboC::CScytheDissComboC()
	: CState(SCYTHEDISSCOMBOC)
	, m_bCombo(false)
{
}

CScytheDissComboC::~CScytheDissComboC()
{
}

void CScytheDissComboC::finaltick()
{
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
		ChangeState(L"ScytheDiss_ComboD");
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CScytheDissComboC::Enter()
{
	GamePlayStatic::Play2DSound(L"sound\\scythe\\diss\\PC_Atk_Flail_Combo3_01.wav", 1, 0.25f);
	GetOwner()->Animator2D()->Play(L"ScytheDiss_ComboC", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	// camera
	FCamEffect Shake {};
	Shake.Type = CAMEFFECT_TYPE::SHAKE;
	Shake.fPlayTime = -1.f;
	Shake.fAccTime = 0.f;
	Shake.fVar = 3.f;
	CGameMgr::GetMainCamera()->GetScriptByType<CCamCtrlScript>()->PushEffect(Shake);

	// hitbox
	m_pHitbox = GetOwner()->GetChildByName(L"ComboC_Hitbox");
	SetPlayerHitboxDamage(m_pHitbox, SCYTHE_DAMAGE_NAME::Air);
	m_pHitbox->Collider2D()->Activate();

	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Scale{ 370, 340, 1 };
	Vec3 Offset{0, 40, 0};

	if (PLAYERSCRIPT->GetDir() == UNIT_DIRX::LEFT)
	{
		Scale.x *= -1.f;
		Offset.x *= -1.f;
	}

	m_pHitbox->Transform()->SetRelativeScale(Scale);
	m_pHitbox->Transform()->SetRelativePos(Offset);
}

void CScytheDissComboC::Exit()
{
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_pHitbox->Collider2D()->Deactivate();

	CGameMgr::GetMainCamera()->GetScriptByType<CCamCtrlScript>()->PopEffect();

	// combo
	PLAYERSCRIPT->StartCombo(2);
	m_bCombo = false;
}
