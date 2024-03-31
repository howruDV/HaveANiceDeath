#include "pch.h"
#include "CUnitScript.h"

#include <Engine/CAnimator2D.h>

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
}