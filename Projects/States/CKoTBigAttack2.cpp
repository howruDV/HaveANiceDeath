#include "pch.h"
#include "CKoTBigAttack2.h"

#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>

#include <Scripts\CMonsterScript.h>

CKoTBigAttack2::CKoTBigAttack2()
	: CState(KOTBIGATTACK2)
	, m_SoundPlay{0,}
{
}

CKoTBigAttack2::~CKoTBigAttack2()
{
}

void CKoTBigAttack2::finaltick()
{
	CGameObject* Hitbox = GetOwner()->GetChildByName(L"Attack2_Hitbox");
	if (!Hitbox || !Hitbox->Collider2D()->IsActive())
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

		if (!m_SoundPlay[3])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_hammer\\NPC_TimeHammer_VO_Atk_Spin_01.wav", 1, 0.2f);
			m_SoundPlay[3] = true;
		}
	}
	else if (CurFrame >= 35 && CurFrame <= 62)
	{
		Scale = Vec3(560, 230, 0);
		Offset = Vec3(0, 0, 0);

		if (!m_SoundPlay[2])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_hammer\\NPC_TimeHammer_Atk_Spin_01.wav", 1, 0.25f);
			m_SoundPlay[2] = true;
		}
	}
	else if (CurFrame >= 27 && CurFrame <= 34)
	{
		Scale = Vec3(290, 230, 0);
		Offset = Vec3(145, 0, 0);

		if (!m_SoundPlay[1])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_hammer\\NPC_TimeHammer_Atk_Counter_Hold_01.wav", 1, 0.25f);
			m_SoundPlay[1] = true;
		}
	}
	else if (CurFrame >= 21 && CurFrame <= 24)
	{
		Scale = Vec3(230, 190, 0);
		Offset = Vec3(165, 0, 0);

		if (!m_SoundPlay[0])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_hammer\\NPC_TimeHammer_Atk_Counter_01.wav", 1, 0.25f);
			GamePlayStatic::Play2DSound(L"sound\\npc_time_hammer\\NPC_TimeHammer_VO_Atk_Counter_01.wav", 1, 0.2f);
			m_SoundPlay[0] = true;
		}
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
	MONSTERSCRIPT->SetDirLock(true);
	GetOwner()->GetChildByName(L"Attack2_Hitbox")->Collider2D()->Activate();

	GetOwner()->Animator2D()->Play(L"Attack2", false);
	GamePlayStatic::Play2DSound(L"sound\\npc_time_hammer\\NPC_TimeHammer_Atk_Counter_Hold_Prepa_01.wav", 1, 0.25f);

	for (bool& iter : m_SoundPlay)
		iter = false;
}

void CKoTBigAttack2::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
	GetOwner()->GetChildByName(L"Attack2_Hitbox")->Collider2D()->Deactivate();
	MONSTERSCRIPT->StartAttackCool();
}