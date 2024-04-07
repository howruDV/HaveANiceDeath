#include "pch.h"
#include "CScytheDissComboA.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissComboA::CScytheDissComboA()
	: CState(SCYTHEDISSCOMBOA)
	, m_bCombo(false)
{
}

CScytheDissComboA::~CScytheDissComboA()
{
}

void CScytheDissComboA::finaltick()
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

	// change state
	if (m_bCombo)
	{
		ChangeState(L"ScytheDiss_ComboB");
		return;
	}

	ChangeState(L"Idle");
}

void CScytheDissComboA::Enter()
{
	GetOwner()->Animator2D()->Play(L"ScytheDiss_ComboA", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CScytheDissComboA::Exit()
{
	PLAYERSCRIPT->StartCombo(0);
	m_bCombo = false;
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}
