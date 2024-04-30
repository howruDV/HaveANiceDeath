#include "pch.h"
#include "CPlayerHit.h"

#include <Engine\CTransform.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CGameMgr.h>
#include <Scripts\CCamCtrlScript.h>

CPlayerHit::CPlayerHit()
	: CState(PLAYERHIT)
{
	m_EffectHit = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\pref_EffectParticle_PlayerHit.pref");
}

CPlayerHit::~CPlayerHit()
{
}

void CPlayerHit::finaltick()
{
	// turn 중이면 state 변경 보류
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	GetFSM()->SetGlobalState(false);
	ChangeState(L"Idle");
}

void CPlayerHit::Enter()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	PLAYERSCRIPT->SetDirLock(true);
	GetFSM()->SetGlobalState(true);

	// anim
	GetOwner()->Animator2D()->Play(L"Hit", false);

	// camera
	FCamEffect Shake{};
	Shake.Type = CAMEFFECT_TYPE::SHAKE;
	Shake.fPlayTime = 0.3f;
	Shake.fAccTime = 0.f;
	Shake.fVar = 5.f;
	CGameMgr::GetMainCamera()->GetScriptByType<CCamCtrlScript>()->SetEffect(Shake);
	
	// sound
	wstring strName = L"sound\\player\\Onos_Player_Death_Hit_light_0";
	int rand = Random(1, 9);
	strName += std::to_wstring(rand) + L".wav";
	GamePlayStatic::Play2DSound(strName, 1, 0.5f);

	// play effect
	// effect: unit anim (explosion)
	CGameObject* pEffect = PLAYERSCRIPT->GetEffect();
	pEffect->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1, 0, 0, 1));
	pEffect->Animator2D()->Play(L"Hit", false);
	pEffect->Animator2D()->DeactiveAfterPlay();
	pEffect->Activate();

	// effect: particle
	Vec3 Pos = GetOwner()->Transform()->GetRelativePos();
	Pos.z = - 0.1f;
	pEffect = m_EffectHit->Instantiate();
	pEffect->Transform()->SetRelativePos(Pos);
	pEffect->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1, 0, 0, 1));
	pEffect->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	GamePlayStatic::SpawnGameObject(pEffect, 30);
}

void CPlayerHit::Exit()
{
	PLAYERSCRIPT->SetDirLock(false);
}