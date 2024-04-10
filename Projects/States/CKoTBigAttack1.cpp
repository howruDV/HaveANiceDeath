#include "pch.h"
#include "CKoTBigAttack1.h"

#include <Engine/CCollider2D.h>

CKoTBigAttack1::CKoTBigAttack1()
	: CState(KOTBIGATTACK1)
	, m_pHitbox(nullptr)
{
}

CKoTBigAttack1::~CKoTBigAttack1()
{
}

void CKoTBigAttack1::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CKoTBigAttack1::Enter()
{
	m_pHitbox = GetOwner()->GetChildByName(L"Attack1_Hitbox");
	m_pHitbox->Activate();

	GetOwner()->Animator2D()->Play(L"Attack2", false);
}

void CKoTBigAttack1::Exit()
{
	m_pHitbox->Deactivate();
}