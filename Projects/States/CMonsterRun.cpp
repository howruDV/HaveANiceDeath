#include "pch.h"
#include "CMonsterRun.h"
#include "func.h"

#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>

#include <Scripts\CMonsterScript.h>

CMonsterRun::CMonsterRun()
	: CState(MONSTERRUN)
{
}

CMonsterRun::~CMonsterRun()
{
}

void CMonsterRun::finaltick()
{
	CMonsterScript* pMonster = MONSTERSCRIPT;
	float DetectRange = *((float*)GetBlackboardData(L"fDetectRange"));
	float AttackRange = *((float*)GetBlackboardData(L"fAttackRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"pTarget"));
	float Speed = *((float*)GetBlackboardData(L"fSpeed"));
	bool Flying = *((float*)GetBlackboardData(L"bFlying"));

	// move
	Vec3 vDir = pTarget->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
	if (!Flying) vDir.y = 0.f;
	vDir.Normalize();

	GetOwner()->Movement()->AddForce(vDir);
	if (vDir.x < 0)
		MONSTERSCRIPT->SetDir(UNIT_DIRX::LEFT);
	else
		MONSTERSCRIPT->SetDir(UNIT_DIRX::RIGHT);

	// range & dir check
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

void CMonsterRun::Enter()
{
	GetOwner()->Animator2D()->Play(L"Run", true);
}

void CMonsterRun::Exit()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
}