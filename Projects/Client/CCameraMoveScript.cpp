#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine/CCamera.h>
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CKeyMgr.h>

CCameraMoveScript::CCameraMoveScript()
	: CScript(-1)
	, m_CamSpeed(300.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::MoveOrthographic()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::UP))
	{
		vPos.y += DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos.y -= DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos.x -= DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos.x += DT_ENGINE * m_CamSpeed;
	}

	Transform()->SetRelativePos(vPos);
}

void CCameraMoveScript::MovePerspective()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);	// 카메라의 현재 방향으로 이동해야 하므로
	Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);

	if (KEY_PRESSED(KEY::UP))
	{
		vPos += DT_ENGINE * m_CamSpeed * vFront;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos -= DT_ENGINE * m_CamSpeed * vFront;
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos -= DT_ENGINE * m_CamSpeed * vRight;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos += DT_ENGINE * m_CamSpeed * vRight;
	}

	Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.x += vDrag.y * DT_ENGINE * XM_PI * 4.f;
		vRot.y += vDrag.x * DT_ENGINE * XM_PI * 4.f;

		Transform()->SetRelativeRotation(vRot);
	}
}

void CCameraMoveScript::tick()
{
	PROJ_TYPE ProjType = Camera()->GetProjType();

	// switch projection type
	if (KEY_TAP(KEY::P))
	{
		if (ProjType == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		else
		{
			Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
			Transform()->SetRelativeRotation(Vec3());	// camera dir 초기화
		}
	}

	// camera control
	if (ProjType == PROJ_TYPE::ORTHOGRAPHIC)
		MoveOrthographic();
	else
		MovePerspective();

	// camera setting control
	if (KEY_PRESSED(KEY::_1))
	{
		if (ProjType == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() + DT_ENGINE * 0.1f);
		else
			Camera()->SetFOV(Camera()->GetFOV() + DT_ENGINE * 2.f);
	}

	if (KEY_PRESSED(KEY::_2))
	{
		if (ProjType == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() - DT_ENGINE * 0.1f);
		else
			Camera()->SetFOV(Camera()->GetFOV() - DT_ENGINE * 2.f);
	}
}