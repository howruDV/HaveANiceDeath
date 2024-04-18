#include "pch.h"
#include "CKoTSpearAttack3.h"

#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>

#include <Scripts\CMonsterScript.h>

CKoTSpearAttack3::CKoTSpearAttack3()
	: CState(KOTSPEARATTACK3)
	, m_SoundPlay{false, }
{
}

CKoTSpearAttack3::~CKoTSpearAttack3()
{
}

void CKoTSpearAttack3::finaltick()
{
	CGameObject* pHitbox = GetOwner()->GetChildByName(L"Attack3_Hitbox");
	if (!pHitbox || !pHitbox->Collider2D()->IsActive())
		return;

	// Update HitBox Transform
 	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if(CurFrame >= 17 && CurFrame <= 23)
	{
		Scale = Vec3(580, 225, 0);
		Offset = Vec3(240, 0, 0);

		if (!m_SoundPlay[0])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_spear\\NPC_TimeSpear_Vo_Atk_Hvy_01.wav", 1, 0.25f);
			m_SoundPlay[0] = true;
		}
	}
	else if (CurFrame >= 10 && CurFrame <= 12)
	{
		Scale = Vec3(550, 200, 0);
		Offset = Vec3(225, 100, 0);

		if (!m_SoundPlay[1])
		{
			wstring strName = L"sound\\npc_time_spear\\NPC_TimeSpear_Vo_Atk_Lgt_0";
			int rand = Random(1, 3);
			strName += std::to_wstring(rand) + L".wav";

			GamePlayStatic::Play2DSound(strName, 1, 0.25f);
			m_SoundPlay[1] = true;
		}
	}
	else if (CurFrame >= 5 && CurFrame <= 6)
	{
		Scale = Vec3(415, 210, 0);
		Offset = Vec3(208, 0, 0);

		if (!m_SoundPlay[0])
		{
			wstring strName = L"sound\\npc_time_spear\\NPC_TimeSpear_Vo_Atk_Lgt_0";
			int rand = Random(1, 3);
			strName += std::to_wstring(rand) + L".wav";

			GamePlayStatic::Play2DSound(strName, 1, 0.25f);
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

void CKoTSpearAttack3::Enter()
{
	GetOwner()->GetChildByName(L"Attack3_Hitbox")->Collider2D()->Activate();
	GetOwner()->Animator2D()->Play(L"Attack3", false);

	for (bool& iter : m_SoundPlay)
		iter = false;
}

void CKoTSpearAttack3::Exit()
{
	GetOwner()->GetChildByName(L"Attack3_Hitbox")->Collider2D()->Deactivate();
	MONSTERSCRIPT->StartAttackCool();
}