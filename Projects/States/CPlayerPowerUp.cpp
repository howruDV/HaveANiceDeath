#include "pch.h"
#include "CPlayerPowerUp.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CAnim.h>
#include <Engine/CCollider2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerPowerUp::CPlayerPowerUp()
	: CState(PLAYERPOWERUP)
	, m_MoveTop(Vec3(0, 200, 0))
{
}

CPlayerPowerUp::~CPlayerPowerUp()
{
}

void CPlayerPowerUp::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		// »ó½Â
		if (GetOwner()->Animator2D()->GetCurAnimFrmIdx() < 28)
		{
			if (PLAYERSCRIPT->IsAirCol())
			{
				CGameObject* CollisionPlatform = PLAYERSCRIPT->GetAirColPlatform();
				m_TargetPos.y = CollisionPlatform->Collider2D()->GetFinalPos().y - CollisionPlatform->Collider2D()->GetFinalScale().y;
			}

			Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
			Vec3 vRemain = m_TargetPos - vPos;

			vPos.y += vRemain.y * 100 * DT;
			GetOwner()->Transform()->SetRelativePos(vPos);
		}
		// ÇÏ°­
		else
		{
			Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
			Vec3 vRemain = m_LandPos - vPos;

			vPos.y += vRemain.y * 50 * DT;
			GetOwner()->Transform()->SetRelativePos(vPos);
		}

		return;
	}

	ChangeState(L"Idle");
}

void CPlayerPowerUp::Enter()
{
	GetOwner()->Movement()->UseGravity(false);
	m_LandPos = GetOwner()->Transform()->GetRelativePos();
	m_TargetPos = m_LandPos + m_MoveTop;

	GetOwner()->Animator2D()->Play(L"PowerUp", false);
}

void CPlayerPowerUp::Exit()
{
	GetOwner()->Movement()->UseGravity(true);
}