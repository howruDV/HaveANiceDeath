#include "pch.h"
#include "CScytheDissComboD.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CScytheDissComboD::CScytheDissComboD()
	: CState(SCYTHEDISSCOMBOD)
{
}

CScytheDissComboD::~CScytheDissComboD()
{
}


void CScytheDissComboD::finaltick()
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

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CScytheDissComboD::Enter()
{
	GetOwner()->Animator2D()->Play(L"ScytheDiss_ComboD", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CScytheDissComboD::Exit()
{
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	PLAYERSCRIPT->DeactiveCombo();
}