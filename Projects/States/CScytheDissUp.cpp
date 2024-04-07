#include "pch.h"
#include "CScytheDissUp.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CAnim.h>
#include <Engine/CCollider2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissUp::CScytheDissUp()
	: CState(SCYTHEDISSUP)
	, m_MoveTop(Vec3(0, 400, 0))
{
}

CScytheDissUp::~CScytheDissUp()
{
}

void CScytheDissUp::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		if (GetOwner()->Animator2D()->GetCurAnimFrmIdx() < 5)
		{
			Vec3 vPos = m_LandPos;
			vPos.y += 180;
			GetOwner()->Transform()->SetRelativePos(vPos);
			return;
		}
		// »ó½Â
		else
		{
			if (PLAYERSCRIPT->IsAirCol())
			{
				CGameObject* CollisionWall = PLAYERSCRIPT->GetAirColPlatform();
				m_TargetPos.y = CollisionWall->Collider2D()->GetFinalPos().y - CollisionWall->Collider2D()->GetFinalScale().y;
				GetOwner()->Transform()->SetRelativePos(m_TargetPos);
				return;
			}

			Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
			Vec3 vRemain = m_TargetPos - vPos;

			vPos.y += vRemain.y * 100 * DT;
			GetOwner()->Transform()->SetRelativePos(vPos);
		}

		return;
	}

	// change state
	if (GetOwner()->Movement()->IsGround())
		ChangeState(L"Idle");
	else
		ChangeState(L"Jump_Falling");
}

void CScytheDissUp::Enter()
{
	GetOwner()->Movement()->UseGravity(false);
	m_LandPos = GetOwner()->Transform()->GetRelativePos();
	m_TargetPos = m_LandPos + m_MoveTop;

	GetOwner()->Animator2D()->Play(L"ScytheDiss_Up", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CScytheDissUp::Exit()
{
	GetOwner()->Movement()->UseGravity(true);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}
