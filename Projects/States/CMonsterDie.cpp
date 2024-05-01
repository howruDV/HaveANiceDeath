#include "pch.h"
#include "CMonsterDie.h"

#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>

#include <Scripts\CMonsterScript.h>

CMonsterDie::CMonsterDie()
	: CState(MONSTERDIE)
{
	m_EffectParticle = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\pref_EffectParticle_MonsterDie.pref");
	m_EffectAnim = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\pref_EffectAnim_MonsterDie.pref");
}

CMonsterDie::~CMonsterDie()
{
}

void CMonsterDie::finaltick()
{
	// play animation
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	GamePlayStatic::DestroyGameObject(GetOwner());
}

void CMonsterDie::Enter()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	MONSTERSCRIPT->SetDirLock(true);
	GetFSM()->SetGlobalState(true);

	// anim
	GetOwner()->Animator2D()->Play(L"Die", false);

	// play effect
	Vec3 Pos = GetOwner()->Transform()->GetRelativePos();
	Pos.z -= 0.1f;

	// effect: particle spawn
	CGameObject* pEffect = m_EffectParticle->Instantiate();
	pEffect->Transform()->SetRelativePos(Pos);
	pEffect->Animator2D()->Play(L"Ghost1", false);
	GamePlayStatic::SpawnGameObject(pEffect, 30);

	// effect: anim spawn
	pEffect = m_EffectAnim->Instantiate();
	pEffect->Transform()->SetRelativePos(Pos);
	pEffect->Animator2D()->Play(L"Die", false);
	pEffect->Animator2D()->DestroyAfterPlay();
	GamePlayStatic::SpawnGameObject(pEffect, 30);

	// effect: player anim
	pEffect = MONSTERSCRIPT->GetEffect();
	if (pEffect)
	{
		pEffect->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		pEffect->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1, 0, 0, 1));
		pEffect->Animator2D()->Play(L"Explosion", false);
		pEffect->Animator2D()->DeactiveAfterPlay();
		pEffect->Activate();
	}
}

void CMonsterDie::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
}