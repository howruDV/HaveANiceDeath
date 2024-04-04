#include "pch.h"
#include "CUnitScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CStateMachine.h>

CUnitScript::CUnitScript(UINT m_iScriptType)
	: CScript(m_iScriptType)
	, m_fSpeed(100.f)
	, m_Dir(UNIT_DIRX::RIGHT)
	, m_Dir_Prev(m_Dir)
	, m_iHPMax(100)
	, m_iHPCur(m_iHPMax)
	, m_bDirLock(false)
	, m_bDirChange_Next(false)
	, m_bDirChange_Cur(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "HP Max", &m_iHPMax);
	AddScriptParam(SCRIPT_PARAM::INT, "HP Current", &m_iHPCur);
}

CUnitScript::CUnitScript(const CUnitScript& _Origin)
	: CScript(_Origin)
	, m_fSpeed(_Origin.m_fSpeed)
	, m_Dir(_Origin.m_Dir)
	, m_Dir_Prev(m_Dir)
	, m_iHPMax(_Origin.m_iHPMax)
	, m_iHPCur(_Origin.m_iHPCur)
	, m_bDirLock(false)
	, m_bDirChange_Next(false)
	, m_bDirChange_Cur(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "HP Max", &m_iHPMax);
	AddScriptParam(SCRIPT_PARAM::INT, "HP Current", &m_iHPCur);
}

CUnitScript::~CUnitScript()
{
}

void CUnitScript::tick()
{
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

	// HP check
	if (m_iHPCur > m_iHPMax)
		m_iHPCur = m_iHPMax;

	else if (m_iHPCur <= 0)
	{
		m_iHPCur = 0;
		//StateMachine()->GetFSM()->ChangeState(L"Death");
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
	//vector<CGameObject*> m_CollisionGround;
}

void CUnitScript::LoadFromFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
	fread(&m_Dir, sizeof(int), 1, _File);
	fread(&m_iHPMax, sizeof(int), 1, _File);
	fread(&m_iHPCur, sizeof(int), 1, _File);
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