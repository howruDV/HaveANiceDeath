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

	// ī�޶� ������ ��
	// background x�� �ݴ�� ������ (�̹� �����۾��� y��� X...)

	// ����ī�޶� ��ġ ��ȭ���� �ִٸ�
	// -��ġ��ȭ�� * (����)�̵��ӵ� * z�� �ݿ���
	// Transform ����

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