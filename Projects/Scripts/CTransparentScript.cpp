#include "pch.h"
#include "CTransparentScript.h"

#include <Engine\CTimeMgr.h>

CTransparentScript::CTransparentScript()
	: CScript(TRANSPARENTSCRIPT)
	, m_TargetTrans(0.f)
	, m_Speed(1.f)
	, m_FadeIn(false)
	, m_AfterDestroy(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Target Transparent", &m_TargetTrans);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_Speed);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Fade In", &m_FadeIn);
	AddScriptParam(SCRIPT_PARAM::BOOL, "After Destroy", &m_AfterDestroy);
}

CTransparentScript::CTransparentScript(const CTransparentScript& _Origin)
	: CScript(TRANSPARENTSCRIPT)
	, m_TargetTrans(_Origin.m_TargetTrans)
	, m_Speed(_Origin.m_Speed)
	, m_FadeIn(_Origin.m_FadeIn)
	, m_AfterDestroy(_Origin.m_AfterDestroy)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Target Transparent", &m_TargetTrans);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_Speed);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Fade In", &m_FadeIn);
	AddScriptParam(SCRIPT_PARAM::BOOL, "After Destroy", &m_AfterDestroy);
}

CTransparentScript::~CTransparentScript()
{
}

void CTransparentScript::tick()
{
	float CurTrans = *(float*)GetRenderComponent()->GetMaterial()->GetScalarParam(SCALAR_PARAM::FLOAT_1);

	// m_TargetTrans는 1에 가까울수록 투명함
	if ((m_FadeIn && m_TargetTrans >= CurTrans)
		|| (!m_FadeIn && m_TargetTrans <= CurTrans))
		return;

	if (m_FadeIn)
		CurTrans -= m_Speed * DT;
	else
		CurTrans += m_Speed * DT;

	if (CurTrans >= m_TargetTrans)
	{
		CurTrans = m_TargetTrans;
		
		if (m_AfterDestroy)
			GamePlayStatic::DestroyGameObject(GetOwner());
	}

	GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, CurTrans);
}

void CTransparentScript::SaveToFile(FILE* _File)
{
	fwrite(&m_TargetTrans, sizeof(float), 1, _File);
	fwrite(&m_Speed, sizeof(float), 1, _File);
	fwrite(&m_FadeIn, sizeof(bool), 1, _File);
	fwrite(&m_AfterDestroy, sizeof(bool), 1, _File);
}

void CTransparentScript::LoadFromFile(FILE* _File)
{
	fread(&m_TargetTrans, sizeof(float), 1, _File);
	fread(&m_Speed, sizeof(float), 1, _File);
	fread(&m_FadeIn, sizeof(bool), 1, _File);
	fread(&m_AfterDestroy, sizeof(bool), 1, _File);
}