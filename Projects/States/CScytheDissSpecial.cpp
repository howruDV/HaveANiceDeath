#include "pch.h"
#include "CScytheDissSpecial.h"

#include <Engine/CTransform.h>
#include <Engine/CCollider2D.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissSpecial::CScytheDissSpecial()
	: CState(SCYTHEDISSSPECIAL)
	, m_SoundIdx(0)
{
	wstring strName = L"sound\\scythe\\diss\\PC_Atk_Flail_Lvl3_Proj_Down_0";

	for (int i = 1; i <= 4; i++)
	{
		Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(strName + std::to_wstring(i) + L".wav");
		if (pSound != nullptr)
			m_vecSound.push_back(pSound);
	}
}

CScytheDissSpecial::~CScytheDissSpecial()
{
}

void CScytheDissSpecial::finaltick()
{	
	// Play Sound
	if (!m_vecSound[m_SoundIdx].Get()->IsPlaying())
	{
		if (++m_SoundIdx > 4)
			m_SoundIdx = 0;

		m_vecSound[m_SoundIdx].Get()->Play(1, 0.4f);
	}
	
	// Update HitBox Transform
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();
	Vec3 Offset;
	Vec3 Scale;
	
	if (CurFrame >= 22 && CurFrame <= 28)
	{
		Scale = Vec3(274, 144, 0);
		Offset = Vec3(137, 0, 0);
	}
	else if (CurFrame >= 7 && CurFrame <= 21)
	{
		Scale = Vec3(614, 224, 0);
		Offset = Vec3();
	}
	else if (CurFrame >= 3 && CurFrame <= 6)
	{
		Scale = Vec3(272, 222, 0);
		Offset = Vec3(0, 0, 0);
	}
	else if (CurFrame <= 2)
	{
		Scale = Vec3(178, 130, 0);
		Offset = Vec3(-89, 0, 0);
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
		return;
	}

	ChangeState(L"Idle");
}

void CScytheDissSpecial::Enter()
{
	m_OrginScale = GetOwner()->Transform()->GetRelativeScale();
	Vec3 Scale = m_OrginScale * 1.2f;
	GetOwner()->Transform()->SetRelativeScale(Scale);

	m_SoundIdx = 0;
	m_vecSound[0].Get()->Play(1, 0.4f);
	GetOwner()->Animator2D()->Play(L"ScytheDiss_Special", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	// hitbox
	m_pHitbox = GetOwner()->GetChildByName(L"Special_Hitbox");
	SetPlayerHitboxDamage(m_pHitbox, SCYTHE_DAMAGE_NAME::Air);
	m_pHitbox->Collider2D()->Activate();
}

void CScytheDissSpecial::Exit()
{
	GetOwner()->Transform()->SetRelativeScale(m_OrginScale);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_pHitbox->Collider2D()->Deactivate();
}