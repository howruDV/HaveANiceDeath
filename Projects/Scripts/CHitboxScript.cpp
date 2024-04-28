#include "pch.h"
#include "CHitboxScript.h"
#include "CUnitScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CHitboxScript::CHitboxScript()
	: CScript(HITBOXSCRIPT)
	, m_HostileLayer(-1)
	, m_Damage{}
	, m_fCoolTime(1.f)
	, m_bRepeat(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Hostile Layer", &m_HostileLayer);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Damage(ActiveHP)", &m_Damage.iActiveHPDamage);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Damage(CurHP)", &m_Damage.iCurHPDamage);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack CoolTime", &m_fCoolTime);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Attack Repeat", &m_bRepeat);
}

CHitboxScript::CHitboxScript(const CHitboxScript& _Origin)
	: CScript(_Origin)
	, m_HostileLayer(_Origin.m_HostileLayer)
	, m_Damage(_Origin.m_Damage)
	, m_fCoolTime(_Origin.m_fCoolTime)
	, m_bRepeat(_Origin.m_bRepeat)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Hostile Layer", &m_HostileLayer);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Damage(MaxHP)", &m_Damage.iActiveHPDamage);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Damage(CurHP)", &m_Damage.iCurHPDamage);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack CoolTime", &m_fCoolTime);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Attack Repeat", &m_bRepeat);
}

CHitboxScript::~CHitboxScript()
{
}

void CHitboxScript::begin()
{
	// change layer
	int Layer = GetOwner()->GetLayerIdx();

	if (Layer == 0 && GetOwner()->GetParent())
	{
		Layer = GetOwner()->GetParent()->GetLayerIdx();
		GetOwner()->ChangeLayer(Layer);
	}

	// change hositle layer
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (m_HostileLayer != -1)
		return;
	else if (Layer == pCurLevel->GetLayerIdxByName(L"Player"))
		m_HostileLayer = pCurLevel->GetLayerIdxByName(L"Monster");
	else
		m_HostileLayer = pCurLevel->GetLayerIdxByName(L"Player");
}

void CHitboxScript::tick()
{
	if (!m_vecAttackHistory.empty())
	{
		vector<FAttackHistory>::iterator it = m_vecAttackHistory.begin();

		for (; it<m_vecAttackHistory.end();)
		{
			it->fAccTime += DT;

			if (it->fAccTime >= m_fCoolTime)
				it = m_vecAttackHistory.erase(it);
			else
				it++;
		}
	}
}

void CHitboxScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	Attack(GetOwner(), _OtherObj);
}

void CHitboxScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (m_bRepeat)
	{
		Attack(GetOwner(), _OtherObj);
	}
}

void CHitboxScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CHitboxScript::SaveToFile(FILE* _File)
{
	fwrite(&m_HostileLayer, sizeof(int), 1, _File);
	fwrite(&m_Damage, sizeof(FDamage), 1, _File);
	fwrite(&m_fCoolTime, sizeof(float), 1, _File);
	fwrite(&m_bRepeat, sizeof(bool), 1, _File);
}

void CHitboxScript::LoadFromFile(FILE* _File)
{
	fread(&m_HostileLayer, sizeof(int), 1, _File);
	fread(&m_Damage, sizeof(FDamage), 1, _File);
	fread(&m_fCoolTime, sizeof(float), 1, _File);
	fread(&m_bRepeat, sizeof(bool), 1, _File);
}

void CHitboxScript::Attack(CGameObject* pInstigator, CGameObject* pTarget)
{
	// check Hostile
	CUnitScript* pAttackTarget = pTarget->GetScriptByType<CUnitScript>();
	if (pTarget->GetLayerIdx() != m_HostileLayer || !pAttackTarget)
		return;

	// case: 이전에 충돌하고 아직 cooltime만큼 지나지 않은 경우
	for (const FAttackHistory& it : m_vecAttackHistory)
	{
		if (it.pTarget == pTarget)
			return;
	}

	// attack
	CUnitScript* pInstigatorUnit;
	if (pInstigator->GetParent())
	{
		if (pInstigator->GetParent()->GetScriptByType<CUnitScript>())
			pInstigatorUnit = pInstigator->GetParent()->GetScriptByType<CUnitScript>();
		else
			pInstigatorUnit = pInstigator->GetScriptByType<CUnitScript>();
	}
	else
	{
		pInstigatorUnit = pInstigator->GetScriptByType<CUnitScript>();
	}

	if (pInstigatorUnit)
		pInstigatorUnit->Attack();
	pAttackTarget->HitDamage(m_Damage);

	// push vector
	FAttackHistory AttackRec = {};
	AttackRec.pTarget = pTarget;
	AttackRec.fAccTime = 0.f;

	if (m_fCoolTime != 0.f)
		m_vecAttackHistory.push_back(AttackRec);
}