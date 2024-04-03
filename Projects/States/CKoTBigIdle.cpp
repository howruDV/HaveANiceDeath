#include "pch.h"
#include "CKoTBigIdle.h"

#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>

CKoTBigIdle::CKoTBigIdle()
	: CState(KOTBIGIDLE)
{
}

CKoTBigIdle::~CKoTBigIdle()
{
}

void CKoTBigIdle::finaltick()
{
	float DetectRange = *((float*)GetBlackboardData(L"fDetectRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"pTarget"));
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos() + GetOwner()->Collider2D()->GetOffsetPos();
	Vec3 vDist = pTarget->Transform()->GetWorldPos() - vPos;

	GamePlayStatic::DrawDebugCircle(vPos, DetectRange, Vec3(0.1f, 1.f, 0.1f), false);

	// change state
	if (vDist.Length() <= DetectRange)
	{
		ChangeState(L"Attack2");
	}
}

void CKoTBigIdle::Enter()
{
	GetOwner()->Animator2D()->Play(L"Idle", true);
}

void CKoTBigIdle::Exit()
{
}