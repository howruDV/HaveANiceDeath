#include "pch.h"
#include "CScytheDissUp.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>
#include <Engine/CAnim.h>

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
	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;

	if (CurFrame >= 10)
	{
		Scale = Vec3(302, 302, 0);
		Offset = Vec3();
	}
	else if (CurFrame >= 6 && CurFrame <= 9)
	{
		Scale = Vec3(400, 488, 0);
		Offset = Vec3(0, -150, 0);
	}
	else if (CurFrame == 5)
	{
		Scale = Vec3(208, 396, 0);
		Offset = Vec3();
	}
	else if (CurFrame <= 4)
	{
		Scale = Vec3(174, 370, 0);
		Offset = Vec3(0, 185, 0);
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
		// »ó½Â
		if (CurFrame >= 5)
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
	GetOwner()->Movement()->SetVelocity(Vec3());
	m_TargetPos = GetOwner()->Transform()->GetRelativePos() + m_MoveTop;

	GamePlayStatic::Play2DSound(L"sound\\scythe\\diss\\PC_Atk_Flail_Lvl3_Proj_Up_03.wav", 1, 0.25f);
	GetOwner()->Animator2D()->Play(L"ScytheDiss_Up", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	// hitbox
	m_pHitbox = GetOwner()->GetChildByName(L"Up_Hitbox");
	SetPlayerHitboxDamage(m_pHitbox, SCYTHE_DAMAGE_NAME::Air);
	m_pHitbox->Collider2D()->Activate();
}

void CScytheDissUp::Exit()
{
	GetOwner()->Movement()->UseGravity(true);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_pHitbox->Collider2D()->Deactivate();
}
