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
	, m_bDirChange(false)
{
}

CUnitScript::~CUnitScript()
{
}

void CUnitScript::tick()
{
	// case: direction unlocked
	if (!m_bDirLock)
	{
		if (m_Dir_Prev != m_Dir)
		{
			m_bDirChange = true;

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
		else
			m_bDirChange = false;

		m_Dir_Prev = m_Dir;
	}

	// case: direction locked
	else
		m_bDirChange = false;
}