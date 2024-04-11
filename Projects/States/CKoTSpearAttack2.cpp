#include "pch.h"
#include "CKoTSpearAttack2.h"

#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>

#include <Scripts\CMonsterScript.h>

CKoTSpearAttack2::CKoTSpearAttack2()
	: CState(KOTSPEARATTACK2)
{
}

CKoTSpearAttack2::~CKoTSpearAttack2()
{
}

void CKoTSpearAttack2::finaltick()
{
	CGameObject* pHitbox = GetOwner()->GetChildByName(L"Attack2_Hitbox");
	if (!pHitbox || !pHitbox->IsActivate())
		return;

	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if (CurFrame >= 11 && CurFrame <= 14)
	{
		Scale = Vec3(345, 275, 0);
		Offset = Vec3(175, -185, 0);
	}

	if (MONSTERSCRIPT->GetDir() == UNIT_DIRX::LEFT)
	{
		Scale.x *= -1.f;
		Offset.x *= -1.f;
	}

	pHitbox->Transform()->SetRelativeScale(Scale);
	pHitbox->Transform()->SetRelativePos(Offset);

	// Add Force
	if (CurFrame == 7)
	{
		Vec3 vForce{ 1000.f, 1000.f, 0 };
		if (MONSTERSCRIPT->GetDir() == UNIT_DIRX::LEFT)
			vForce.x *= -1;

		GetOwner()->Movement()->SetVelocity(vForce);
	}

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CKoTSpearAttack2::Enter()
{
	GetOwner()->GetChildByName(L"Attack2_Hitbox")->Activate();
	GetOwner()->Animator2D()->Play(L"Attack4", false);
}

void CKoTSpearAttack2::Exit()
{
	GetOwner()->GetChildByName(L"Attack2_Hitbox")->Deactivate();
	GetOwner()->Movement()->SetVelocity(Vec3());
	MONSTERSCRIPT->StartAttackCool();
}
