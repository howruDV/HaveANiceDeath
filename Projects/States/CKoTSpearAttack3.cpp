#include "pch.h"
#include "CKoTSpearAttack3.h"

#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>

#include <Scripts\CMonsterScript.h>

CKoTSpearAttack3::CKoTSpearAttack3()
	: CState(KOTSPEARATTACK3)
{
}

CKoTSpearAttack3::~CKoTSpearAttack3()
{
}

void CKoTSpearAttack3::finaltick()
{
	CGameObject* pHitbox = GetOwner()->GetChildByName(L"Attack3_Hitbox");
	if (!pHitbox || !pHitbox->IsActivate())
		return;

	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if(CurFrame >= 17 && CurFrame <= 23)
	{
		Scale = Vec3(580, 225, 0);
		Offset = Vec3(240, 0, 0);
	}
	else if (CurFrame >= 10 && CurFrame <= 12)
	{
		Scale = Vec3(550, 200, 0);
		Offset = Vec3(225, 100, 0);
	}
	else if (CurFrame >= 5 && CurFrame <= 6)
	{
		Scale = Vec3(415, 210, 0);
		Offset = Vec3(208, 0, 0);
	}

	if (MONSTERSCRIPT->GetDir() == UNIT_DIRX::LEFT)
	{
		Scale.x *= -1.f;
		Offset.x *= -1.f;
	}

	pHitbox->Transform()->SetRelativeScale(Scale);
	pHitbox->Transform()->SetRelativePos(Offset);

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CKoTSpearAttack3::Enter()
{
	GetOwner()->GetChildByName(L"Attack3_Hitbox")->Activate();
	GetOwner()->Animator2D()->Play(L"Attack3", false);
}

void CKoTSpearAttack3::Exit()
{
	GetOwner()->GetChildByName(L"Attack3_Hitbox")->Deactivate();
	MONSTERSCRIPT->StartAttackCool();
}
