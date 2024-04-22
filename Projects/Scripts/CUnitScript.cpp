#include "pch.h"
#include "CUnitScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CStateMachine.h>
#include <Engine/CCollider2D.h>

CUnitScript::CUnitScript(UINT m_iScriptType)
	: CScript(m_iScriptType)
	, m_fSpeed(100.f)
	, m_Dir(UNIT_DIRX::RIGHT)
	, m_Dir_Prev(m_Dir)
	, m_Dir_Next(m_Dir)
	, m_iHPMax(100)
	, m_iHPCur(m_iHPMax)
	, m_bDirLock(false)
	, m_bDirChange_Next(false)
	, m_bDirChange_Cur(false)
	, m_fStunTime(3.f)
	, m_fStunCoolAcc(0.f)
	, m_bStun(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "HP Max", &m_iHPMax);
	AddScriptParam(SCRIPT_PARAM::INT, "HP Current", &m_iHPCur);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Stun Time", &m_fStunTime);
}

CUnitScript::CUnitScript(const CUnitScript& _Origin)
	: CScript(_Origin)
	, m_fSpeed(_Origin.m_fSpeed)
	, m_Dir(_Origin.m_Dir)
	, m_Dir_Prev(m_Dir)
	, m_Dir_Next(m_Dir)
	, m_iHPMax(_Origin.m_iHPMax)
	, m_iHPCur(_Origin.m_iHPCur)
	, m_bDirLock(false)
	, m_bDirChange_Next(false)
	, m_bDirChange_Cur(false)
	, m_fStunTime(3.f)
	, m_fStunCoolAcc(0.f)
	, m_bStun(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "HP Max", &m_iHPMax);
	AddScriptParam(SCRIPT_PARAM::INT, "HP Current", &m_iHPCur);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Stun Time", &m_fStunTime);
}

CUnitScript::~CUnitScript()
{
}

void CUnitScript::begin()
{
	vector<CGameObject*> childs = GetOwner()->GetChild();
	for (CGameObject* iter : childs)
	{
		if (iter->Collider2D())
			iter->Collider2D()->Deactivate();
	}
}

void CUnitScript::tick()
{
	// ----------------------------
	// check direction
	// ----------------------------
	m_bDirChange_Next = false;
	m_bDirChange_Cur = false;

	// case: direction unlocked
	if (!m_bDirLock)
	{
		if (m_Dir_Prev != m_Dir || m_Dir != m_Dir_Next)
		{
			if (m_Dir != m_Dir_Next)
				m_bDirChange_Next = true;
			if (m_Dir != m_Dir_Prev)
				m_bDirChange_Cur = true;

			if (GetOwner()->Animator2D())
			{
				GetOwner()->Animator2D()->SetFlipX(m_Dir);
			}
			else
			{
				// @TODO 아 ㅋㅋ 쓸때추가해ㅋㅋ
				//GetOwner()->GetRenderComponent()->GetDynamicMaterial
			}
		}

		m_Dir_Prev = m_Dir;
		m_Dir = m_Dir_Next;
	}

	// ----------------------------
	// check state
	// ----------------------------

	// HP check
	if (m_iHPCur > m_iHPMax)
		m_iHPCur = m_iHPMax;

	else if (m_iHPCur <= 0)
	{
		m_iHPCur = 0;

		if (StateMachine() && StateMachine()->GetFSM()->FindState(L"Die"))
			StateMachine()->GetFSM()->ChangeState(L"Die");
	}

	if (m_bStun)
	{
		m_fStunCoolAcc += DT;

		if (m_fStunCoolAcc >= m_fStunTime)
		{
			m_fStunCoolAcc = 0.f;
			m_bStun = false;
		}
	}
}

void CUnitScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (m_CollisionGround.empty())
		Movement()->SetGround(false);
}

void CUnitScript::SaveToFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
	fwrite(&m_Dir, sizeof(int), 1, _File);
	fwrite(&m_iHPMax, sizeof(int), 1, _File);
	fwrite(&m_iHPCur, sizeof(int), 1, _File);
	fwrite(&m_bDirLock, sizeof(bool), 1, _File);
	//vector<CGameObject*> m_CollisionGround;
}

void CUnitScript::LoadFromFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
	fread(&m_Dir, sizeof(int), 1, _File);
	fread(&m_iHPMax, sizeof(int), 1, _File);
	fread(&m_iHPCur, sizeof(int), 1, _File);
	fread(&m_bDirLock, sizeof(bool), 1, _File);

	m_Dir_Prev = m_Dir;
	m_Dir_Next = m_Dir;
}

void CUnitScript::HitDamage(FDamage _Damage)
{ 
	if (StateMachine() && StateMachine()->GetFSM()->FindState(L"Hit"))
		StateMachine()->GetFSM()->ChangeState(L"Hit");

	m_iHPCur -= _Damage.iCurHPDamage;
}

void CUnitScript::DeleteGround(CGameObject* _Ground)
{
	vector<CGameObject*>::iterator iter = find(m_CollisionGround.begin(), m_CollisionGround.end(), _Ground);
	m_CollisionGround.erase(iter);
}

bool CUnitScript::IsGround(CGameObject* _Platform)
{
	vector<CGameObject*>::iterator iter = find(m_CollisionGround.begin(), m_CollisionGround.end(), _Platform);

	if (iter != m_CollisionGround.end())
		return true;
	return false;
}

bool CUnitScript::IsOverlapGround(CGameObject* _pObject)
{
	vector<CGameObject*>::iterator iter = m_CollisionGround.begin();

	for (; iter != m_CollisionGround.end(); ++iter)
	{
		if (*iter == _pObject)
		{
			return true;
		}
	}

	return false;
}
