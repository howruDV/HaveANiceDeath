#include "pch.h"
#include "CPlayerRun.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMovement.h>
#include <Engine/CTransform.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerRun::CPlayerRun()
	:CState(PLAYERRUN)
{
}

CPlayerRun::~CPlayerRun()
{
}

void CPlayerRun::finaltick()
{
	// Run
	float fSpeed = *((float*)GetBlackboardData(L"fSpeed"));

	if (KEY_PRESSED(KEY::A))
	{
		Vec3 vSpeed = Vec3(-fSpeed, 0, 0);
		GetOwner()->Movement()->AddForce(vSpeed);
		CPlayerMgr::GetInst()->GetPlayerScript()->SetLookLeft(true);
	}
	if (KEY_PRESSED(KEY::D))
	{
		Vec3 vSpeed = Vec3(fSpeed, 0, 0);
		GetOwner()->Movement()->AddForce(vSpeed);
		CPlayerMgr::GetInst()->GetPlayerScript()->SetLookLeft(false);
	}
	
	// Change State
	// 다른 방향키가 같이 탭되면, 일시정지(Idle)했다가, 풀리면 눌린 쪽으로 다시 이동
	if ((KEY_PRESSED(KEY::A) && KEY_PRESSED(KEY::D))
		|| (KEY_RELEASED(KEY::A) || KEY_RELEASED(KEY::D)))
	{
		GetOwner()->Animator2D()->Play(L"Run_ToIdle", false);
		ChangeState(L"Idle");
	}
}

void CPlayerRun::Enter()
{
	//Vec3 Scale = GetOwner()->Transform()->GetRelativeScale(); Scale.x *= -1;
	//GetOwner()->Transform()->SetRelativeScale(Scale);
	GetOwner()->Animator2D()->PushNextAnim(L"Run");
}

void CPlayerRun::Exit()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	GetOwner()->Animator2D()->ClearNextAnim();
}