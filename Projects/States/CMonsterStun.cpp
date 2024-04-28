#include "pch.h"
#include "CMonsterStun.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CMovement.h>
#include <Engine\CTransform.h>
#include <Engine\CStatemachine.h>

#include <Scripts\CMonsterScript.h>
#include <Scripts\CPlayerMgr.h>

CMonsterStun::CMonsterStun()
	: CState(MONSTERSTUN)
	, m_fHitBlinkTime(0.5f)
{
}

CMonsterStun::~CMonsterStun()
{
}

void CMonsterStun::finaltick()
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

	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	GetFSM()->SetGlobalState(false);
	ChangeState(L"Idle");
}

void CMonsterStun::Enter()
{
	//MONSTERSCRIPT->StartStun();
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
	GetOwner()->Animator2D()->Play(L"Stun", false);
}

void CMonsterStun::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
	GetFSM()->SetGlobalState(false);

	GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(0.f));
	GetOwner()->Movement()->SetVelocity(Vec3());
	GetOwner()->Movement()->SetFrictionScale(0.f);
}