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
	, m_fMaxDepth(1200.f)
	, m_MainCamCtrlr(nullptr)
	, m_bAirPerspective(true)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Max Depth", &m_fMaxDepth);
	AddScriptParam(SCRIPT_PARAM::VEC4, "Air Color", &m_vAirColor);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Use AirPerspective", &m_bAirPerspective);
}

CParallelScript::CParallelScript(const CParallelScript& _Origin)
	: CScript(PARALLELSCRIPT)
	, m_vAirColor(_Origin.m_vAirColor)
	, m_fMaxDepth(_Origin.m_fMaxDepth)
	, m_MainCamCtrlr(nullptr)
	, m_bAirPerspective(_Origin.m_bAirPerspective)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Max Depth", &m_fMaxDepth);
	AddScriptParam(SCRIPT_PARAM::VEC4, "Air Color", &m_vAirColor);
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
		m_MainCamCtrlr = pLevel->FindObjectByName(L"MainCamera", 0)->GetScriptByType<CCamCtrlScript>();
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
	Vec3 vUpdatePos = GetOwner()->Transform()->GetRelativePos();
	if (vCamMove == Vec3() || vUpdatePos.z == 0.f)
		return;
	
	// case: z > 0
	// - 값이 커질수록: 카메라의 진행방향으로 밀어줌
	// - (멀리 있을수록, 카메라가 움직여도 이미지는 고정된 것처럼 보임)
	if (vUpdatePos.z > 0)
 		vUpdatePos.x += vCamMove.x * (vUpdatePos.z / m_fMaxDepth);

	// case: z < 0
	// - 값이 작을수록 : 카메라 진행방향의 반대로 밀어줌
	// - (가까이 있을수록, 카메라가 조금 움직여도 이미지의 변화값이 큼)
	else
		vUpdatePos.x += vCamMove.x / m_fMaxDepth * (1.f / vUpdatePos.z);

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