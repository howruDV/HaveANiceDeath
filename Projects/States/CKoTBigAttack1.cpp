#include "pch.h"
#include "CKoTBigAttack1.h"

#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>

#include <Scripts\CMonsterScript.h>

CKoTBigAttack1::CKoTBigAttack1()
	: CState(KOTBIGATTACK1)
{
}

CKoTBigAttack1::~CKoTBigAttack1()
{
}

void CKoTBigAttack1::finaltick()
{
	CGameObject* pHitbox = GetOwner()->GetChildByName(L"Attack1_Hitbox");
	if (!pHitbox || !pHitbox->Collider2D()->IsActive())
		return;

	// Update HitBox Transform
	wstring strName = GetOwner()->GetName();
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if (CurFrame >= 13 && CurFrame <= 17)
	{
		Scale = Vec3(430, 200, 0);
		Offset = Vec3(215, 0, 0);
	}
	else if (CurFrame >= 11 && CurFrame <= 12)
	{
		Scale = Vec3(640, 480, 0);
		Offset = Vec3(125, 120, 0);

		if (!m_SoundPlay)
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_hammer\\NPC_TimeHammer_Atk_Ground_01.wav", 1, 0.25f);
			GamePlayStatic::Play2DSound(L"sound\\npc_time_hammer\\NPC_TimeHammer_VO_Atk_Hvy_01.wav", 1, 0.2f);
			m_SoundPlay = true;
		}
	}
	else if (CurFrame >= 3 && CurFrame <= 7)
	{
		Scale = Vec3(410, 150, 0);
		Offset = Vec3(205, 0, 0);
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

void CKoTBigAttack1::Enter()
{
	GetOwner()->GetChildByName(L"Attack1_Hitbox")->Collider2D()->Activate();
	GetOwner()->Animator2D()->Play(L"Attack1", false);
	GamePlayStatic::Play2DSound(L"sound\\npc_time_hammer\\NPC_TimeHammer_Atk_Ground_Prepa_01.wav", 1, 0.2f);

	m_SoundPlay = false;
}

void CKoTBigAttack1::Exit()
{
	GetOwner()->GetChildByName(L"Attack1_Hitbox")->Collider2D()->Deactivate();
	MONSTERSCRIPT->StartAttackCool();
}