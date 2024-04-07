#include "pch.h"
#include "CScytheDissComboC.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissComboC::CScytheDissComboC()
	: CState(SCYTHEDISSCOMBOC)
	, m_bCombo(false)
{
}

CScytheDissComboC::~CScytheDissComboC()
{
}

void CScytheDissComboC::finaltick()
{
	if (KEY_TAP(KEY::SPACE) || KEY_PRESSED(KEY::SPACE))
	{
		ChangeState(L"Jump_Start");
	}

	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}

	if (KEY_TAP(KEY::LBTN))
		m_bCombo = true;

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	if (m_bCombo)
	{
		ChangeState(L"ScytheDiss_ComboD");
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CScytheDissComboC::Enter()
{
	GetOwner()->Animator2D()->Play(L"ScytheDiss_ComboC", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CScytheDissComboC::Exit()
{
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	PLAYERSCRIPT->StartCombo(2);
	m_bCombo = false;
}
