#include "pch.h"
#include "CParallelScript.h"
#include "CCamCtrlScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

CParallelScript::CParallelScript()
	: CScript(PARALLELSCRIPT)
	, m_vAirColor(Vec4(174, 184, 169, 255))
	, m_fSpeed(50.f)
	, m_MainCamCtrlr(nullptr)
	, m_bAirPerspective(true)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::VEC4, "Air Color", &m_vAirColor);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Use AirPerspective", &m_bAirPerspective);
}

CParallelScript::CParallelScript(const CParallelScript& _Origin)
	: CScript(PARALLELSCRIPT)
	, m_vAirColor(_Origin.m_vAirColor)
	, m_fSpeed(_Origin.m_fSpeed)
	, m_MainCamCtrlr(nullptr)
	, m_bAirPerspective(_Origin.m_bAirPerspective)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::VEC4, "Merge Color", &m_vAirColor);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Use AirPerspective", &m_bAirPerspective);
}


CParallelScript::~CParallelScript()
{
}

void CParallelScript::begin()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pLevel)
	{
		int LayerIdx = pLevel->GetLayerIdxByName(L"Default");
		m_MainCamCtrlr = pLevel->FindObjectByName(L"MainCamera", LayerIdx)->GetScriptByType<CCamCtrlScript>();
	}

	Ptr<CMaterial> pMat = GetOwner()->MeshRender()->GetMaterial();
	pMat->SetScalarParam(SCALAR_PARAM::INT_0, m_bAirPerspective);
	pMat->SetScalarParam(SCALAR_PARAM::VEC4_0, m_vAirColor/255.f);
	pMat->SetScalarParam(SCALAR_PARAM::FLOAT_0, Transform()->GetRelativePos().z);
}

void CParallelScript::tick()
{
	if (!m_MainCamCtrlr)
		return;

	Vec3 vCamMove = m_MainCamCtrlr->GetMove();
	if (vCamMove == Vec3())
		return;

	Vec3 vUpdatePos = GetOwner()->Transform()->GetRelativePos();
	vUpdatePos.x -= vCamMove.x * m_fSpeed * (1 / vUpdatePos.z);
	GetOwner()->Transform()->SetRelativePos(vUpdatePos);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, vUpdatePos.z); // @TODO : release, 혹은 개발 완료시 지우기
}

void CParallelScript::SaveToFile(FILE* _File)
{
	fwrite(&m_vAirColor, 1, sizeof(Vec4), _File);
}

void CParallelScript::LoadFromFile(FILE* _File)
{
	fread(&m_vAirColor, 1, sizeof(Vec4), _File);
}