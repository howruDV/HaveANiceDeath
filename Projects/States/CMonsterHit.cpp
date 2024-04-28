#include "pch.h"
#include "CMonsterHit.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CMovement.h>
#include <Engine\CTransform.h>
#include <Engine\CStatemachine.h>

#include <Scripts\CMonsterScript.h>
#include <Scripts\CPlayerMgr.h>

CMonsterHit::CMonsterHit()
	: CState(MONSTERHIT)
	, m_fHitBlinkTime(0.5f)
{
}

CMonsterHit::~CMonsterHit()
{
}


void CMonsterHit::finaltick()
{
	static float HitAccTime = 0.f;
	HitAccTime = *((float*)GetBlackboardData(L"fHitAccTime"));
	HitAccTime += DT;
	GetOwner()->StateMachine()->AddBlackboardData(L"fHitAccTime", BB_DATA::FLOAT, &HitAccTime);

	if (HitAccTime > m_fHitBlinkTime)
	{
		GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4());
		GetOwner()->Movement()->SetVelocity(Vec3());
	}

	// play animation
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	GetFSM()->SetGlobalState(false);
	ChangeState(L"Idle");
}

void CMonsterHit::Enter()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	MONSTERSCRIPT->SetDirLock(true);
	GetFSM()->SetGlobalState(true);

	// set blackboard data
	float HitAccTime = 0.f;
	GetOwner()->StateMachine()->AddBlackboardData(L"fHitAccTime", BB_DATA::FLOAT, &HitAccTime);

	// push back
	Vec3 Dir = GetOwner()->Transform()->GetWorldPos() - CPlayerMgr::GetPlayer()->Transform()->GetWorldPos();
	Dir = Dir.Normalize();
	GetOwner()->Movement()->AddForce(Dir * 100.f);
	GetOwner()->Movement()->SetFrictionScale(50.f);

	// material
	GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(1.f, 0.f, 0.f, 0.3f));

	// animation
	GetOwner()->Animator2D()->Play(L"Hit", false);
}

void CMonsterHit::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
	GetFSM()->SetGlobalState(false);

	GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(0.f));
	GetOwner()->Movement()->SetVelocity(Vec3());
	GetOwner()->Movement()->SetFrictionScale(0.f);
}