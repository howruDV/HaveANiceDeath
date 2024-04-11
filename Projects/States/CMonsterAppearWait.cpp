#include "pch.h"
#include "CMonsterAppearWait.h"

#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>

#include <Scripts\CMonsterScript.h>

CMonsterAppearWait::CMonsterAppearWait()
	: CState(MONSTERAPPEARWAIT)
{
}

CMonsterAppearWait::~CMonsterAppearWait()
{
}

void CMonsterAppearWait::finaltick()
{
	CMonsterScript* pMonster = MONSTERSCRIPT;
	float DetectRange = *((float*)GetBlackboardData(L"fDetectRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"pTarget"));

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos() + GetOwner()->Collider2D()->GetOffsetPos();
	Vec3 vDist = pTarget->Transform()->GetWorldPos() - vPos;

	// change state
	if (vDist.Length() <= DetectRange)
	{
		ChangeState(L"Appear");
	}
}

void CMonsterAppearWait::Enter()
{
	MONSTERSCRIPT->SetDirLock(true);
}

void CMonsterAppearWait::Exit()
{
	MONSTERSCRIPT->SetDirLock(false);
}
