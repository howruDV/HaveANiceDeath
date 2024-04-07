#include "pch.h"
#include "CScytheDissCrush.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>

CScytheDissCrush::CScytheDissCrush()
	: CState(SCYTHEDISSCRUSH)
{
}

CScytheDissCrush::~CScytheDissCrush()
{
}

void CScytheDissCrush::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		if (GetOwner()->Animator2D()->GetCurAnimFrmIdx() < 5)
		{
			if (GetOwner()->Movement()->IsGround() && m_LandingPos == Vec3())
				m_LandingPos = GetOwner()->Transform()->GetRelativePos();

			Vec3 vPos = m_StartPos;
			vPos.y -= 260;
			GetOwner()->Transform()->SetRelativePos(vPos);
			return;
		}
		// ÇÏ°­
		else
		{
			if (GetOwner()->Movement()->IsGround())
			{
				if (m_LandingPos != Vec3())
					GetOwner()->Transform()->SetRelativePos(m_LandingPos);
				return;
			}

			Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
			vPos.y -= 1000.f * DT;
			GetOwner()->Transform()->SetRelativePos(vPos);
		}

		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CScytheDissCrush::Enter()
{
	m_StartPos = GetOwner()->Transform()->GetRelativePos();
	GetOwner()->Animator2D()->Play(L"ScytheDiss_Crush", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CScytheDissCrush::Exit()
{
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_LandingPos = Vec3();
}