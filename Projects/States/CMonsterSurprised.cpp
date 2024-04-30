#include "pch.h"
#include "CMonsterSurprised.h"
#include "func.h"

#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>

#include <Scripts\CMonsterScript.h>

CMonsterSurprised::CMonsterSurprised()
	: CState(MONSTERSURPRISED)
{
	m_EffectSurprised = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\pref_EffectAnim_Surprised.pref");
}

CMonsterSurprised::~CMonsterSurprised()
{
}

void CMonsterSurprised::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	CMonsterScript* pMonster = MONSTERSCRIPT;
	float DetectRange = *((float*)GetBlackboardData(L"fDetectRange"));
	float AttackRange = *((float*)GetBlackboardData(L"fAttackRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"pTarget"));

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos() + GetOwner()->Collider2D()->GetOffsetPos();
	Vec3 vDist = pTarget->Transform()->GetWorldPos() - vPos;
	UNIT_DIRX ToPlayerLook = (vDist.x < 0) ? UNIT_DIRX::LEFT : UNIT_DIRX::RIGHT;
	GamePlayStatic::DrawDebugCircle(vPos, AttackRange, Vec3(0.1f, 1.f, 0.1f), false);

	// change state
	if (vDist.Length() > DetectRange)
	{
		ChangeState(L"Idle");
		return;
	}
	else if (vDist.Length() <= DetectRange && vDist.Length() > AttackRange)
	{
		ChangeState(L"Run");
	}
	else if (vDist.Length() <= AttackRange)
	{
		if (pMonster->CanAttack())
		{
			wstring strName = L"Attack";
			int rand = Random(1, pMonster->GetAttackTypeCount());
			strName += std::to_wstring(rand);

			ChangeState(strName);
		}
		else
			ChangeState(L"Idle");
	}

	if (pMonster->GetDir() != ToPlayerLook)
	{
		if (MONSTERSCRIPT->WillDirChange())
			ChangeState(L"UTurn");
	}
}

void CMonsterSurprised::Enter()
{
	MONSTERSCRIPT->SetDirLock(true);
	GetOwner()->Animator2D()->Play(L"Surprised", false);

	// play effect
	Vec3 Pos = Vec3();
	Pos.y += GetOwner()->Transform()->GetRelativeScale().y / 2.f + 10.f;
	Pos.z = -1.f;

	// effect: above head
	CGameObject* pEffect = m_EffectSurprised->Instantiate();
	pEffect->Transform()->SetRelativePos(Pos);
	pEffect->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(1.f, 0.2f, 0.2f, 1.f));
	pEffect->Animator2D()->Play(L"Surprised", false);
	pEffect->Animator2D()->DestroyAfterPlay();
	GamePlayStatic::SpawnChild(GetOwner(), pEffect, 30);

	// effect: unit effect
	pEffect = MONSTERSCRIPT->GetEffect();
	if (pEffect)
	{
		pEffect->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		pEffect->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1, 0, 0, 1));
		pEffect->Animator2D()->Play(L"Surprised", false);
		pEffect->Animator2D()->DeactiveAfterPlay();

		Vec3 Scale = pEffect->Transform()->GetRelativeScale();
		float ScaleFactor = GetOwner()->Transform()->GetRelativeScale().x / Scale.x;
		Scale.x *= ScaleFactor;
		Scale.y *= ScaleFactor;

		pEffect->Transform()->SetRelativeScale(Scale);
		pEffect->Activate();
	}
}

void CMonsterSurprised::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
}