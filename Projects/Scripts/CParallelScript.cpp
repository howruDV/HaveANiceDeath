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
	, m_vColor(Vec4(174, 184, 169, 255))
	, m_fSpeed(50.f)
	, m_MainCamCtrlr(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::VEC4, "Merge Color", &m_vColor);
}

CParallelScript::CParallelScript(const CParallelScript& _Origin)
	: CScript(PARALLELSCRIPT)
	, m_vColor(_Origin.m_vColor)
	, m_fSpeed(_Origin.m_fSpeed)
	, m_MainCamCtrlr(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::VEC4, "Merge Color", &m_vColor);
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
}

void CParallelScript::SaveToFile(FILE* _File)
{
	fwrite(&m_vColor, 1, sizeof(Vec4), _File);
}

void CParallelScript::LoadFromFile(FILE* _File)
{
	fread(&m_vColor, 1, sizeof(Vec4), _File);
}