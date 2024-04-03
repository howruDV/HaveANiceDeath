#include "pch.h"
#include "CHitboxScript.h"
#include "CUnitScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CHitboxScript::CHitboxScript()
	: CScript(HITBOXSCRIPT)
	, m_HitTarget(nullptr)
	, m_HostileLayer(-1)
	, m_iDamage(0)
	, m_fCoolTime(0.f)
	, m_fAccTime(0.f)
	, m_bRepeat(false)
{
}

CHitboxScript::CHitboxScript(const CHitboxScript& _Origin)
	: CScript(_Origin)
	, m_HitTarget(nullptr)
	, m_HostileLayer(_Origin.m_HostileLayer)
	, m_iDamage(_Origin.m_iDamage)
	, m_fCoolTime(_Origin.m_fCoolTime)
	, m_fAccTime(0.f)
	, m_bRepeat(_Origin.m_bRepeat)
{
}

CHitboxScript::~CHitboxScript()
{
}

void CHitboxScript::begin()
{
	// change layer
	int Layer = GetOwner()->GetParent()->GetLayerIdx();
	GetOwner()->ChangeLayer(Layer);

	// change hositle layer
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (m_HostileLayer != -1)
		return;
	else if (Layer == pCurLevel->GetLayerIdxByName(L"Player"))
		m_HostileLayer = pCurLevel->GetLayerIdxByName(L"Monster");
	else
		m_HostileLayer = pCurLevel->GetLayerIdxByName(L"Player");
}

void CHitboxScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	m_HitTarget = _OtherObj->GetScriptByType<CUnitScript>();
	if (_OtherObj->GetLayerIdx() != m_HostileLayer || !m_HitTarget)
		return;

	m_HitTarget->GetDamage(m_iDamage);
	m_fAccTime = 0.f;
}

void CHitboxScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (!m_HitTarget)
		return;

	if (m_bRepeat)
	{
		m_fAccTime += DT;

		if (m_fAccTime >= m_fCoolTime)
		{
			m_HitTarget->GetDamage(m_iDamage);
			m_fAccTime = 0.f;
		}
	}
}

void CHitboxScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	m_HitTarget = nullptr;
}

void CHitboxScript::SaveToFile(FILE* _File)
{
	fwrite(&m_HostileLayer, sizeof(int), 1, _File);
	fwrite(&m_iDamage, sizeof(int), 1, _File);
	fwrite(&m_fCoolTime, sizeof(float), 1, _File);
	fwrite(&m_bRepeat, sizeof(bool), 1, _File);
}

void CHitboxScript::LoadFromFile(FILE* _File)
{
	fread(&m_HostileLayer, sizeof(int), 1, _File);
	fread(&m_iDamage, sizeof(int), 1, _File);
	fread(&m_fCoolTime, sizeof(float), 1, _File);
	fread(&m_bRepeat, sizeof(bool), 1, _File);
}