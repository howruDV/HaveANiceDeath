#include "pch.h"
#include "CKoTBigAttack2.h"

#include <Engine/CCollider2D.h>

CKoTBigAttack2::CKoTBigAttack2()
	: CState(KOTBIGATTACK2)
	, m_pHitbox(nullptr)
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
	m_pHitbox = GetOwner()->GetChildByName(L"Attack2_Hitbox");
	m_pHitbox->Activate();
	//m_pHitbox->Collider2D()->Activate();

	GetOwner()->Animator2D()->Play(L"Attack2", false);
}

void CKoTBigAttack2::Exit()
{
	m_pHitbox->Deactivate();
}