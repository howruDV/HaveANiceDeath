#include "pch.h"
#include "CKoTSpearAttack2.h"

#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>

#include <Scripts\CMonsterScript.h>

CKoTSpearAttack2::CKoTSpearAttack2()
	: CState(KOTSPEARATTACK2)
	, m_SoundPlay{false, }
{
}

CKoTSpearAttack2::~CKoTSpearAttack2()
{
}

void CKoTSpearAttack2::finaltick()
{
	CGameObject* pHitbox = GetOwner()->GetChildByName(L"Attack2_Hitbox");
	if (!pHitbox || !pHitbox->Collider2D()->IsActive())
		return;

	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if (CurFrame == 17)
	{
		if (!m_SoundPlay[2])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_spear\\NPC_TimeSpear_Land_Lgt_02.wav", 1, 0.25f);
			GamePlayStatic::Play2DSound(L"sound\\npc_time_spear\\NPC_TimeSpear_Vo_Atk_Lgt_01.wav", 1, 0.25f);
			m_SoundPlay[2] = true;
		}
	}
	if (CurFrame >= 11 && CurFrame <= 14)
	{
		Scale = Vec3(345, 275, 0);
		Offset = Vec3(175, -185, 0);

		if (!m_SoundPlay[1])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_spear\\NPC_TimeSpear_Atk_Punch_Whsh_01.wav", 1, 0.25f);
			m_SoundPlay[1] = true;
		}
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

		if (!m_SoundPlay[0])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_spear\\NPC_TimeSpear_Jump_Lgt_01.wav", 1, 0.25f);
			m_SoundPlay[0] = true;
		}
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
	MONSTERSCRIPT->SetDirLock(true);
	GetOwner()->GetChildByName(L"Attack2_Hitbox")->Collider2D()->Activate();
	GetOwner()->Animator2D()->Play(L"Attack4", false);

	for (bool& iter : m_SoundPlay)
		iter = false;
}

void CKoTSpearAttack2::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
	GetOwner()->GetChildByName(L"Attack2_Hitbox")->Collider2D()->Deactivate();
	GetOwner()->Movement()->SetVelocity(Vec3());
	MONSTERSCRIPT->StartAttackCool();
}
