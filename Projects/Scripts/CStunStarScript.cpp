#include "pch.h"
#include "CStunStarScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CTransform.h>

#define PlayTime 1.f

CStunStarScript::CStunStarScript()
	: CScript(STUNSTARSCRIPT)
	, m_Idx(0)
	, m_AccTime(0.f)
	, m_bFadeIn(true)
	, m_bFadeOut(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Star Idx(0~3)", &m_Idx);
}

CStunStarScript::CStunStarScript(const CStunStarScript& _Origin)
	: CScript(STUNSTARSCRIPT)
	, m_Idx(_Origin.m_Idx)
	, m_AccTime(0.f)
	, m_bFadeIn(true)
	, m_bFadeOut(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Star Idx(0~3)", &m_Idx);
}

CStunStarScript::~CStunStarScript()
{
}

void CStunStarScript::begin()
{
	wstring name = L"Star" + std::to_wstring(m_Idx+1);
	Animator2D()->Play(name);

	m_OrgScale = Transform()->GetRelativeScale();
	Transform()->SetRelativeScale(Vec3());
}

void CStunStarScript::tick()
{
	m_AccTime += DT;

	// set pos
	Vec3 Pos = Vec3();
	Pos.x += sinf((m_AccTime + m_Idx/4.f) * XM_PI/2.f * 3.f) * 50.f;
	Pos.y += sinf((m_AccTime + m_Idx/4.f) * XM_PI/2.f * 3.f * 0.5f) * 10.f;
	Pos.z = -0.001 * m_Idx;
	Transform()->SetRelativePos(Pos);

	// set scale
	Vec3 Scale;

	if (m_bFadeIn)
	{
		Scale = Transform()->GetRelativeScale();

		if (Scale.x < m_OrgScale.x && Scale.y < m_OrgScale.y)
			Scale += Vec3(80.f) * DT;
		else
			m_bFadeIn = false;
	}
	else if (m_bFadeOut)
	{
		Scale = Transform()->GetRelativeScale();
		Scale -= Vec3(80.f) * DT;
	}
	else
	{
		Scale = m_OrgScale;
		Scale += Vec3(5.f) * sinf((m_AccTime + m_Idx / 4.f) * XM_PI / 2.f * 3.f);
	}

	Transform()->SetRelativeScale(Scale);

	if (m_AccTime > PlayTime)
		GamePlayStatic::DestroyGameObject(GetOwner());
	else if (m_AccTime > PlayTime - 0.3f)
		m_bFadeOut = true;

}

void CStunStarScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Idx, sizeof(int), 1, _File);
}

void CStunStarScript::LoadFromFile(FILE* _File)
{
	fread(&m_Idx, sizeof(int), 1, _File);
}