#include "pch.h"
#include "CKoTFlyingAttack1.h"

#include <Engine\CAssetMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CCollider2D.h>

#include <Scripts\CMonsterScript.h>

CKoTFlyingAttack1::CKoTFlyingAttack1()
	: CState(KOTFLYINGATTACK1)
	, m_SoundPlay{false, }
{
}

CKoTFlyingAttack1::~CKoTFlyingAttack1()
{
}

void CKoTFlyingAttack1::finaltick()
{
	CGameObject* pHitbox = GetOwner()->GetChildByName(L"Attack1_Hitbox");
	if (!pHitbox || !pHitbox->Collider2D()->IsActive())
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

		if (!m_SoundPlay[1])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_knight\\NPC_Time_Knight_Atk_Whsh_03.wav", 1, 0.25f);
			m_SoundPlay[1] = true;
		}
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

		if (!m_SoundPlay[0])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_knight\\NPC_Time_Knight_Vo_Atk_Short_01.wav", 1, 0.2f);
			m_SoundPlay[0] = true;
		}
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
	MONSTERSCRIPT->SetDirLock(true);
	GetOwner()->GetChildByName(L"Attack1_Hitbox")->Collider2D()->Activate();
	GetOwner()->Animator2D()->Play(L"Attack2", false);

	for (bool& iter : m_SoundPlay)
		iter = false;
}

void CKoTFlyingAttack1::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
	GetOwner()->GetChildByName(L"Attack1_Hitbox")->Collider2D()->Deactivate();
	MONSTERSCRIPT->StartAttackCool();
}