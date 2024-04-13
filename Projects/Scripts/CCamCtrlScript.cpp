#include "pch.h"
#include "CCamCtrlScript.h"
#include "CPlayerMgr.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

CCamCtrlScript::CCamCtrlScript()
	: CScript(CAMCTRLSCRIPT)
	, m_fSpeed(1500.f)
	, m_Target(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed);
}

CCamCtrlScript::CCamCtrlScript(const CCamCtrlScript& _Origin)
	: CScript(CAMCTRLSCRIPT)
	, m_fSpeed(_Origin.m_fSpeed)
	, m_Target(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed);
}

CCamCtrlScript::~CCamCtrlScript()
{
}

void CCamCtrlScript::begin()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pLevel)
	{
		int LayerIdx = pLevel->GetLayerIdxByName(L"Player");
		m_Target = pLevel->FindObjectByName(L"Death", LayerIdx);
		m_vPrevPos = GetOwner()->Transform()->GetWorldPos();
	}
}

void CCamCtrlScript::tick()
{
	if (!m_Target)
	{
		m_vMove = Vec3();
		return;
	}

	Vec3 vTargetPos = m_Target->Transform()->GetWorldPos();
	vTargetPos.z = m_vPrevPos.z;
	if ((vTargetPos - m_vPrevPos).Length() <= 1.f)
	{
		m_vMove = Vec3();
		return;
	}

	Vec3 vCamPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vDir = (vTargetPos - vCamPos).Normalize();
	Vec3 vUpdatePos = vCamPos + vDir * DT * m_fSpeed;

	GetOwner()->Transform()->SetRelativePos(Vec3(vUpdatePos.x, vUpdatePos.y, vCamPos.z));
	m_vMove.x = vUpdatePos.x - vCamPos.x;
	m_vMove.y = vUpdatePos.y - vCamPos.y;
	m_vPrevPos = vUpdatePos;
}

void CCamCtrlScript::SaveToFile(FILE* _File)
{
	fwrite(&m_fSpeed, 1, sizeof(float), _File);
}

void CCamCtrlScript::LoadFromFile(FILE* _File)
{
	fread(&m_fSpeed, 1, sizeof(float), _File);
}