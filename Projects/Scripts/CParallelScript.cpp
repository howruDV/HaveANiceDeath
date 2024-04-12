#include "pch.h"
#include "CParallelScript.h"

#include <Engine\CTransform.h>

CParallelScript::CParallelScript()
	: CScript(PARALLELSCRIPT)
	, m_vColor(Vec4(174, 184, 169, 255))
{
	AddScriptParam(SCRIPT_PARAM::VEC4, "Merge Color", &m_vColor);
}

CParallelScript::CParallelScript(const CParallelScript& _Origin)
	: CScript(PARALLELSCRIPT)
	, m_vColor(_Origin.m_vColor)
{
	AddScriptParam(SCRIPT_PARAM::VEC4, "Merge Color", &m_vColor);
}


CParallelScript::~CParallelScript()
{
}

void CParallelScript::tick()
{
	float depth = GetOwner()->Transform()->GetRelativePos().z;
	float speed = 100.f / depth;

	// 카메라가 움직일 때
	// background x는 반대로 움직임 (이미 에셋작업을 y고려 X...)

	// 메인카메라 위치 변화량이 있다면
	// -위치변화량 * (고정)이동속도 * z축 반영값
	// Transform 적용

	GetOwner()->Transform();
}

void CParallelScript::SaveToFile(FILE* _File)
{
	fwrite(&m_vColor, 1, sizeof(Vec4), _File);
}

void CParallelScript::LoadFromFile(FILE* _File)
{
	fread(&m_vColor, 1, sizeof(Vec4), _File);
}