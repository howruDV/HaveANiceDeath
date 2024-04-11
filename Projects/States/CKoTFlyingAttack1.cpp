#include "pch.h"
#include "CKoTFlyingAttack1.h"

#include <Engine\CAssetMgr.h>
#include <Engine\CTransform.h>

#include <Scripts\CMonsterScript.h>

CKoTFlyingAttack1::CKoTFlyingAttack1()
	: CState(KOTFLYINGATTACK1)
{
}

CKoTFlyingAttack1::~CKoTFlyingAttack1()
{
}

void CKoTFlyingAttack1::finaltick()
{
	CGameObject* pHitbox = GetOwner()->GetChildByName(L"Attack1_Hitbox");
	if (!pHitbox || !pHitbox->IsActivate())
		return;

	// Update HitBox Transform
	wstring strName = GetOwner()->GetName();
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if (CurFrame >= 14 && CurFrame <= 15)
	{
		Scale = Vec3(380, 240, 0);
		Offset = Vec3(0, 40, 0);
	}
	else if (CurFrame >= 10 && CurFrame <= 13)
	{
		Scale = Vec3(930, 350, 0);
		Offset = Vec3(0, 50, 0);
	}
	else if (CurFrame == 9)
	{
		Scale = Vec3(360, 275, 0);
		Offset = Vec3(0, 35, 0);
	}
	else if (CurFrame == 3)
	{
		Scale = Vec3(175, 290, 0);
		Offset = Vec3(87, 60, 0);
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

void CKoTFlyingAttack1::Enter()
{
	GetOwner()->GetChildByName(L"Attack1_Hitbox")->Activate();
	GetOwner()->Animator2D()->Play(L"Attack2", false);
}

void CKoTFlyingAttack1::Exit()
{
	GetOwner()->GetChildByName(L"Attack1_Hitbox")->Deactivate();
	MONSTERSCRIPT->StartAttackCool();
}