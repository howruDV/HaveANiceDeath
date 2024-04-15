#include "pch.h"
#include "CGaugeScript.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CAnimator2D.h>

#define SPEED 0.5f

CGaugeScript::CGaugeScript()
	: CScript(GAUGESCRIPT)
	, m_iCurVar(0.f)
	, m_iTargetVar(0.f)
{
}

CGaugeScript::~CGaugeScript()
{
}

void CGaugeScript::begin()
{
	if (CPlayerMgr::GetPlayer())
	{
		m_iCurVar = m_iTargetVar = CPlayerMgr::GetPlayerScript()->GetRestGauge();
		GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, Vec2(0.f, m_iCurVar - 1.f));
	}

	if (GetOwner()->Animator2D())
		GetOwner()->Animator2D()->Play(L"Gauge");

	GetOwner()->Transform()->SetRelativeScale(Vec3(64.f, 61.6f, 1.f));
}

void CGaugeScript::tick()
{
	if (m_iCurVar == m_iTargetVar)
		return;

	// get gauge
	float fSpeed = (m_iCurVar < m_iTargetVar) ? SPEED : -SPEED;
	float fDelta = fSpeed * DT;

	if (fabs(fDelta) > fabs(m_iTargetVar - m_iCurVar))
		fDelta = m_iTargetVar - m_iCurVar;

	// set guage
	m_iCurVar += fDelta;
	GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, Vec2(0.f, m_iCurVar - 1.f));
}

void CGaugeScript::SaveToFile(FILE* _File)
{
}

void CGaugeScript::LoadFromFile(FILE* _File)
{
}

void CGaugeScript::SetGauge(float _progress)
{
	m_iCurVar = m_iTargetVar = _progress;
	GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, Vec2(0.f, _progress - 1.f));
}

void CGaugeScript::Increase(float _progress)
{
	m_iTargetVar = _progress;
}

void CGaugeScript::Reset()
{
	m_iTargetVar = 0.f;
}
