#include "pch.h"
#include "CKoTBigAttack2.h"

#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>

#include <Scripts\CMonsterScript.h>

CKoTBigAttack2::CKoTBigAttack2()
	: CState(KOTBIGATTACK2)
{
}

CKoTBigAttack2::~CKoTBigAttack2()
{
}

void CKoTBigAttack2::finaltick()
{
	CGameObject* Hitbox = GetOwner()->GetChildByName(L"Attack2_Hitbox");
	if (!Hitbox || !Hitbox->IsActivate())
		return;

	// Update Hitbox Transform
	wstring strName = GetOwner()->GetName();
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if (CurFrame >= 63 && CurFrame <= 73)
	{
		Scale = Vec3(200, 330, 0);
		Offset = Vec3(-100, 0, 0);
	}
	else if (CurFrame >= 35 && CurFrame <= 62)
	{
		Scale = Vec3(560, 230, 0);
		Offset = Vec3(0, 0, 0);
	}
	else if (CurFrame >= 27 && CurFrame <= 34)
	{
		Scale = Vec3(290, 230, 0);
		Offset = Vec3(145, 0, 0);
	}
	else if (CurFrame >= 21 && CurFrame <= 24)
	{
		Scale = Vec3(230, 190, 0);
		Offset = Vec3(165, 0, 0);
	}

	if (MONSTERSCRIPT->GetDir() == UNIT_DIRX::LEFT)
	{
		Scale.x *= -1.f;
		Offset.x *= -1.f;
	}

	Hitbox->Transform()->SetRelativeScale(Scale);
	Hitbox->Transform()->SetRelativePos(Offset);

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CKoTBigAttack2::Enter()
{
	CGameObject* pHitbox = GetOwner()->GetChildByName(L"Attack2_Hitbox");
	pHitbox->Activate();

	GetOwner()->Animator2D()->Play(L"Attack2", false);
}

void CKoTBigAttack2::Exit()
{
	CGameObject* pHitbox = GetOwner()->GetChildByName(L"Attack2_Hitbox");
	pHitbox->Deactivate();
	MONSTERSCRIPT->StartAttackCool();
}