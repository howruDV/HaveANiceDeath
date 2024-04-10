#include "pch.h"
#include "CMonsterIdle.h"
#include "func.h"

#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>

#include <Scripts\CMonsterScript.h>

CMonsterIdle::CMonsterIdle()
	: CState(MONSTERIDLE)
{
}

CMonsterIdle::~CMonsterIdle()
{
}

void CMonsterIdle::finaltick()
{
	CMonsterScript* pMonster = MONSTERSCRIPT;
	float DetectRange = *((float*)GetBlackboardData(L"fDetectRange"));
	float AttackRange = *((float*)GetBlackboardData(L"fAttackRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"pTarget"));

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos() + GetOwner()->Collider2D()->GetOffsetPos();
	Vec3 vDist = pTarget->Transform()->GetWorldPos() - vPos;
	GamePlayStatic::DrawDebugCircle(vPos, AttackRange, Vec3(0.1f, 1.f, 0.1f), false);

	UNIT_DIRX ToPlayerLook = ((pTarget->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos()).x < 0) ? UNIT_DIRX::LEFT : UNIT_DIRX::RIGHT;

	// change state
	if (vDist.Length() <= DetectRange && vDist.Length() > AttackRange)
	{
		ChangeState(L"Surprised");
	}
	
	if (vDist.Length() <= AttackRange)
	{
		if (pMonster->CanAttack())
		{
			wstring strName = GetOwner()->GetName() + L"Attack";
			int rand = Random(1, pMonster->GetAttackTypeCount());
			strName += std::to_wstring(rand);

			ChangeState(strName);
		}
	}

	if (pMonster->GetDir() != ToPlayerLook)
	{
		ChangeState(L"UTurn");
	}
}

void CMonsterIdle::Enter()
{
	GetOwner()->Animator2D()->Play(L"Idle", true);
}

void CMonsterIdle::Exit()
{
}