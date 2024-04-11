#include "pch.h"
#include "CKoTSpear_ProjScript.h"
#include "CMonsterScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CMovement.h>
#include <Engine\CTransform.h>

CKoTSpear_ProjScript::CKoTSpear_ProjScript()
	: CScript(KOTSPEAR_PROJSCRIPT)
	, m_fSpeed(2000.f)
	, m_fAccTime(0.f)
	, m_fLifeTime(3.f)
	, m_Dir(UNIT_DIRX::RIGHT)
{
}

CKoTSpear_ProjScript::~CKoTSpear_ProjScript()
{
}

void CKoTSpear_ProjScript::begin()
{
	// velocity
	if (m_Dir == UNIT_DIRX::LEFT)
	{
		Vec3 vRot{ 0.f, 0.f, 180.f };
		vRot.ToRadian();
		Transform()->SetRelativeRotation(vRot);
		m_fSpeed *= -1.f;
	}

	GetOwner()->Movement()->SetVelocity(Vec3(m_fSpeed, 0, 0));
}

void CKoTSpear_ProjScript::tick()
{
	// Check Time
	m_fAccTime += DT;
	if (m_fAccTime >= m_fLifeTime)
	{
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
}

void CKoTSpear_ProjScript::SaveToFile(FILE* _File)
{
}

void CKoTSpear_ProjScript::LoadFromFile(FILE* _File)
{
}