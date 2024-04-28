#include "pch.h"
#include "CScaleScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CTransform.h>

CScaleScript::CScaleScript()
	: CScript(SCALESCRIPT)
	, m_ScaleSpeed(1.f)
{
	AddScriptParam(SCRIPT_PARAM::VEC3, "Scale Factor", &m_ScaleFactor);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Scale Speed", &m_ScaleSpeed);
}

CScaleScript::CScaleScript(const CScaleScript& _Origin)
	: CScript(SCALESCRIPT)
	, m_OrgScale(_Origin.m_OrgScale)
	, m_ScaleFactor(_Origin.m_ScaleFactor)
	, m_ScaleSpeed(_Origin.m_ScaleSpeed)
{
	AddScriptParam(SCRIPT_PARAM::VEC3, "Scale Factor", &m_ScaleFactor);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Scale Speed", &m_ScaleSpeed);
}

CScaleScript::~CScaleScript()
{
}

void CScaleScript::begin()
{
	m_OrgScale = Transform()->GetRelativeScale();
}

void CScaleScript::tick()
{
	Vec3 MaxScale = m_OrgScale * m_ScaleFactor;
	Vec3 CurScale = Transform()->GetRelativeScale();
	if (CurScale.x >= MaxScale.x && CurScale.y >= MaxScale.y && CurScale.z >= MaxScale.z)
		return;

	Vec3 Scale = CurScale + m_ScaleFactor * m_ScaleSpeed * DT;
	Transform()->SetRelativeScale(Scale);
}

void CScaleScript::SaveToFile(FILE* _File)
{
	fwrite(&m_ScaleFactor, sizeof(Vec3), 1, _File);
	fwrite(&m_ScaleSpeed, sizeof(float), 1, _File);
}

void CScaleScript::LoadFromFile(FILE* _File)
{
	fread(&m_ScaleFactor, sizeof(Vec3), 1, _File);
	fread(&m_ScaleSpeed, sizeof(float), 1, _File);
}
