#include "pch.h"
#include "CKoTBigAttack2.h"

CKoTBigAttack2::CKoTBigAttack2()
	: CState(KOTBIGATTACK2)
{
}

CKoTBigAttack2::~CKoTBigAttack2()
{
}

void CKoTBigAttack2::finaltick()
{
	float Speed = *((float*)GetBlackboardData(L"fAttack2_Damage"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"pTarget"));

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	ChangeState(L"Idle");
}

void CKoTBigAttack2::Enter()
{
	GetOwner()->Animator2D()->Play(L"Attack2", false);
	//GetOwner()->GetChildByName(L"Attack2_Hitbox");
}

void CKoTBigAttack2::Exit()
{
}