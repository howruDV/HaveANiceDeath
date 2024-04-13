#include "pch.h"
#include "CRepeatImgScript.h"

CRepeatImgScript::CRepeatImgScript()
	: CScript(REPEATIMGSCRIPT)
	, m_RepeatX(1)
	, m_RepeatY(1)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Repeat X", &m_RepeatX);
	AddScriptParam(SCRIPT_PARAM::INT, "Repeat Y", &m_RepeatY);
}

CRepeatImgScript::CRepeatImgScript(const CRepeatImgScript& _Origin)
	: CScript(REPEATIMGSCRIPT)
	, m_RepeatX(1)
	, m_RepeatY(1)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Repeat X", &m_RepeatX);
	AddScriptParam(SCRIPT_PARAM::INT, "Repeat Y", &m_RepeatY);
}

CRepeatImgScript::~CRepeatImgScript()
{
}

void CRepeatImgScript::begin()
{
	Ptr<CMaterial> pMat = GetOwner()->MeshRender()->GetMaterial();
	pMat->SetScalarParam(SCALAR_PARAM::INT_0, m_RepeatX);
	pMat->SetScalarParam(SCALAR_PARAM::INT_1, m_RepeatY);
}

void CRepeatImgScript::tick()
{
}

void CRepeatImgScript::SaveToFile(FILE* _File)
{
	fwrite(&m_RepeatX, 1, sizeof(int), _File);
	fwrite(&m_RepeatY, 1, sizeof(int), _File);
}

void CRepeatImgScript::LoadFromFile(FILE* _File)
{
	fread(&m_RepeatX, 1, sizeof(Vec4), _File);
	fread(&m_RepeatY, 1, sizeof(Vec4), _File);
}