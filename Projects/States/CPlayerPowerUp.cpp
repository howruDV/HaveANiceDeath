#include "pch.h"
#include "CPlayerPowerUp.h"

#include <Engine\CAssetMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CAnim.h>
#include <Engine/CCollider2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerPowerUp::CPlayerPowerUp()
	: CState(PLAYERPOWERUP)
	, m_MoveTop(Vec3(0, 200, 0))
	, m_bSpawnEffect(false)
{
	m_bEffectRepulsive = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\pref_EffectAnim_Repulsive.pref");
}

CPlayerPowerUp::~CPlayerPowerUp()
{
}

void CPlayerPowerUp::finaltick()
{
	//play effect
	if (GetOwner()->Animator2D()->GetCurAnimFrmIdx() == 16)
	{
		if (!m_bSpawnEffect)
		{
			Vec3 Pos = m_LandPos;
			Pos.y -= 10.f;
			Pos.z -= 0.1f;

			CGameObject* pEffect = m_bEffectRepulsive->Instantiate();
			pEffect->Transform()->SetRelativePos(Pos);
			pEffect->Animator2D()->Play(L"Repulsive", false);
			pEffect->Animator2D()->DestroyAfterPlay();

			GamePlayStatic::SpawnGameObject(pEffect, 30);
			m_bSpawnEffect = true;
		}
	}

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
	m_bSpawnEffect = false;

	GetOwner()->Animator2D()->Play(L"PowerUp", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CPlayerPowerUp::Exit()
{
	GetOwner()->Movement()->UseGravity(true);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	// off effect
	PLAYERSCRIPT->GetEffect()->Deactivate();
}