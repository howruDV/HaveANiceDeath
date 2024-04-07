#include "pch.h"
#include "CScytheDissComboB.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissComboB::CScytheDissComboB()
	: CState(SCYTHEDISSCOMBOB)
	, m_bCombo(false)
{
}

CScytheDissComboB::~CScytheDissComboB()
{
}

void CScytheDissComboB::finaltick()
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
		ChangeState(L"ScytheDiss_ComboC");
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CScytheDissComboB::Enter()
{
	GetOwner()->Animator2D()->Play(L"ScytheDiss_ComboB", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CScytheDissComboB::Exit()
{
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	PLAYERSCRIPT->StartCombo(1);
	m_bCombo = false;
}