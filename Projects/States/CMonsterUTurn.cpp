#include "pch.h"
#include "CMonsterUTurn.h"

#include <Scripts\CMonsterScript.h>

#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>

CMonsterUTurn::CMonsterUTurn()
	: CState(MONSTERUTURN)
{
}

CMonsterUTurn::~CMonsterUTurn()
{
}

void CMonsterUTurn::finaltick()
{
	float DetectRange = *((float*)GetBlackboardData(L"fDetectRange"));
	float AttackRange = *((float*)GetBlackboardData(L"fAttackRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"pTarget"));
	
	// range & dir check
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos() + GetOwner()->Collider2D()->GetOffsetPos();
	Vec3 vDist = pTarget->Transform()->GetWorldPos() - vPos;
	GamePlayStatic::DrawDebugCircle(vPos, AttackRange, Vec3(0.1f, 1.f, 0.1f), false);

	// Move
	if (vDist.Length() <= DetectRange && vDist.Length() > AttackRange)
	{
		float fSpeed = *((float*)GetBlackboardData(L"fSpeed"));
		Vec3 vForce{ fSpeed, 0.f, 0.f };

		if (MONSTERSCRIPT->GetDir() == UNIT_DIRX::RIGHT)
			vForce.x *= -1;

		GetOwner()->Movement()->AddForce(vForce);
	}

	// change state
	// turn 중이면 state 변경 보류
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	// change state
	ChangeState(L"Idle");
}

void CMonsterUTurn::Enter()
{
	MONSTERSCRIPT->SetDirLock(true);

	// play anim
	GetOwner()->Animator2D()->Play(L"UTurn", false);
}

void CMonsterUTurn::Exit()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	MONSTERSCRIPT->SetDirLock(false);
}