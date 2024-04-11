#include "pch.h"
#include "CBloomScript.h"

CBloomScript::CBloomScript()
	: CScript(BLOOMSCRIPT)
{
	Ptr<CMaterial> pBloomMat = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BloomMat");
	m_bUse = *(int*)pBloomMat->GetScalarParam(SCALAR_PARAM::INT_0);
	m_BloomColor = *(Vec4*)pBloomMat->GetScalarParam(SCALAR_PARAM::VEC4_0);
	m_fThreshold = *(float*)pBloomMat->GetScalarParam(SCALAR_PARAM::FLOAT_0);

	AddScriptParam(SCRIPT_PARAM::BOOL, "Use Bloom", &m_bUse);
	AddScriptParam(SCRIPT_PARAM::VEC4, "Bloom Color", &m_BloomColor);
	AddScriptParam(SCRIPT_PARAM::VEC4, "Threshold", &m_fThreshold);
}

CBloomScript::CBloomScript(const CBloomScript& _Origin)
	: CScript(BLOOMSCRIPT)
	, m_bUse(_Origin.m_bUse)
	, m_BloomColor(_Origin.m_BloomColor)
	, m_fThreshold(_Origin.m_fThreshold)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "Use Bloom", &m_bUse);
	AddScriptParam(SCRIPT_PARAM::VEC4, "Bloom Color", &m_BloomColor);
	AddScriptParam(SCRIPT_PARAM::VEC4, "Threshold", &m_fThreshold);
}

CBloomScript::~CBloomScript()
{
}

void CBloomScript::begin()
{
	Ptr<CMaterial> pMat = GetRenderComponent()->GetDynamicMaterial();
	pMat->SetScalarParam(SCALAR_PARAM::INT_0, m_bUse);
	pMat->SetScalarParam(SCALAR_PARAM::VEC4_0, m_BloomColor);
	pMat->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_fThreshold);
}

void CBloomScript::SaveToFile(FILE* _File)
{
	fwrite(&m_bUse, sizeof(bool), 1, _File);
	fwrite(&m_BloomColor, sizeof(Vec4), 1, _File);
	fwrite(&m_fThreshold, sizeof(float), 1, _File);
}

void CBloomScript::LoadFromFile(FILE* _File)
{
	fread(&m_bUse, sizeof(bool), 1, _File);
	fread(&m_BloomColor, sizeof(Vec4), 1, _File);
	fread(&m_fThreshold, sizeof(float), 1, _File);
}
