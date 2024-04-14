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
	Vec3 vCamPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vUpdatePos;
	vTargetPos.z = vCamPos.z;

	Vec3 vDir = (vTargetPos - vCamPos).Normalize();
	vUpdatePos = vCamPos + vDir * DT * m_fSpeed;
	m_vMove.x = vUpdatePos.x - vCamPos.x;
	m_vMove.y = vUpdatePos.y - vCamPos.y;

	if (m_vMove.Length() > (vTargetPos - vCamPos).Length())
		vUpdatePos = vTargetPos;

	vUpdatePos = CheckCamArea(vUpdatePos);
	m_vMove = vUpdatePos - vCamPos;

	GetOwner()->Transform()->SetRelativePos(Vec3(vUpdatePos.x, vUpdatePos.y, vCamPos.z));
}

void CCamCtrlScript::SaveToFile(FILE* _File)
{
	fwrite(&m_fSpeed, 1, sizeof(float), _File);
}

void CCamCtrlScript::LoadFromFile(FILE* _File)
{
	fread(&m_fSpeed, 1, sizeof(float), _File);
}


Vec3 CCamCtrlScript::CheckCamArea(Vec3 _Pos)
{
	Vec3 ret = _Pos;

	if (_Pos.x < -7200.f)
		ret.x = -7200.f;

	if (_Pos.x > 7120.f)
		ret.x = 7120.f;

	return ret;
}