#include "pch.h"
#include "CTraceState.h"

#include <Engine/CGameObject.h>
#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>

#include <Engine/CTimeMgr.h>

CTraceState::CTraceState()
	: CState(IDLESTATE)
{
}

CTraceState::~CTraceState()
{
}


void CTraceState::finaltick()
{
	float Speed = *((float*)GetBlackboardData(L"Speed"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();
	vDir.Normalize();

	Vec3 vPos = pSelf->Transform()->GetWorldPos() + vDir * DT * Speed;
	pSelf->Transform()->SetRelativePos(vPos);
}

void CTraceState::Enter()
{
	GetFSM()->GetStateMachine()->GetOwner()->Transform()->SetRelativeScale(Vec3(200, 100, 1));
	GetOwner()->Transform()->SetRelativeScale(Vec3(200, 100, 1));
}

void CTraceState::Exit()
{
}