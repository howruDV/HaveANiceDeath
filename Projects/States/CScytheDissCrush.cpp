#include "pch.h"
#include "CScytheDissCrush.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine\CCollider2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissCrush::CScytheDissCrush()
	: CState(SCYTHEDISSCRUSH)
{
}

CScytheDissCrush::~CScytheDissCrush()
{
}

void CScytheDissCrush::finaltick()
{
	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;
	
	if (CurFrame >= 6 && CurFrame <= 11)
	{
		Scale = Vec3(410, 400, 0);
		Offset = Vec3(35, 112, 0);
	}
	else if (CurFrame == 5)
	{
		Scale = Vec3(300, 464, 0);
		Offset = Vec3(0, 108, 0);
	}
	else if (CurFrame >= 2 && CurFrame <= 4)
	{
		Scale = Vec3(300, 464, 0);
		Offset = Vec3(0, -100, 0);
	}
	else if (CurFrame == 0)
	{
		Scale = Vec3(180, 290, 0);
		Offset = Vec3(90, 0, 0);
	}

	if (PLAYERSCRIPT->GetDir() == UNIT_DIRX::LEFT)
	{
		Scale.x *= -1.f;
		Offset.x *= -1.f;
	}

	m_pHitbox->Transform()->SetRelativeScale(Scale);
	m_pHitbox->Transform()->SetRelativePos(Offset);

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		// Adjust Transform
		if (CurFrame < 5)
		{
			GetOwner()->Transform()->SetRelativePos(m_StartPos);
		}
		else
		{
			if (!GetOwner()->Movement()->IsGround())
			{
				Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
				if (CurFrame == 5 && m_bDownFirst)
				{
					// @TODO 터지면 여기, 플레이어가 두 명 이상인 경우 값을 공유하면 안 됨
					vPos.y -= 260.f;
					m_bDownFirst = false;
				}
				vPos.y -= 1000.f * DT;
				GetOwner()->Transform()->SetRelativePos(vPos);
			}
		}

		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CScytheDissCrush::Enter()
{
	m_bDownFirst = true;
	m_StartPos = GetOwner()->Transform()->GetRelativePos();
	GetOwner()->Animator2D()->Play(L"ScytheDiss_Crush", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	// hitbox
	m_pHitbox = GetOwner()->GetChildByName(L"Crush_Hitbox");
	SetPlayerHitboxDamage(m_pHitbox, SCYTHE_DAMAGE_NAME::Air);
	m_pHitbox->Collider2D()->Activate();
}

void CScytheDissCrush::Exit()
{
	GetOwner()->Movement()->SetVelocity(Vec3());
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_pHitbox->Collider2D()->Deactivate();
}