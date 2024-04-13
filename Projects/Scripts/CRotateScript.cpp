#include "pch.h"
#include "CRotateScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CTransform.h>

CRotateScript::CRotateScript()
	: CScript(ROTATESCRIPT)
	, m_fSpeed(0.5f)
	, m_fPos(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Rotate Speed", &m_fSpeed);
}

CRotateScript::CRotateScript(const CRotateScript& _Origin)
	: CScript(ROTATESCRIPT)
	, m_fSpeed(_Origin.m_fSpeed)
	, m_fPos(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Rotate Speed", &m_fSpeed);
}

CRotateScript::~CRotateScript()
{
}

void CRotateScript::begin()
{
	m_fPos = Transform()->GetRelativePos().x;
}

void CRotateScript::tick()
{
	//vScreenUV.y += cos((vScreenUV.x + (g_time * (speed))) * (frequency)) * (vibration);
	//vScreenUV.y += cos((vScreenUV.x + (g_time * 0.1f)) * 40.f) * 0.1f;
	Vec3 vRot = Transform()->GetRelativeRotation();
	float delta = sinf(CTimeMgr::GetInst()->GetGameTime() + m_fPos) * m_fSpeed * XM_PI;
	delta /= 360;
	vRot.z += delta;

	Transform()->SetRelativeRotation(vRot);
}

void CRotateScript::SaveToFile(FILE* _File)
{
	fwrite(&m_fSpeed, 1, 1, _File);
}

void CRotateScript::LoadFromFile(FILE* _File)
{
	fread(&m_fSpeed, 1, 1, _File);
}