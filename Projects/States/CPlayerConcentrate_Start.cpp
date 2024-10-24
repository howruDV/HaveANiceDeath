#include "pch.h"
#include "CPlayerConcentrate_Start.h"

#include <Engine\CTransform.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>

CPlayerConcentrate_Start::CPlayerConcentrate_Start()
	: CState(PLAYERCONCENTRATE_START)
{
}

CPlayerConcentrate_Start::~CPlayerConcentrate_Start()
{
}

void CPlayerConcentrate_Start::finaltick()
{
	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}
	
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		if (KEY_RELEASED(KEY::LBTN) || KEY_NONE(KEY::LBTN))
			ChangeState(L"Idle");

		return;
	}

	// change state
	ChangeState(L"Concentrate");
}

void CPlayerConcentrate_Start::Enter()
{
	GetOwner()->Animator2D()->Play(L"Concentrate_Start", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	// play effect
	CGameObject* pEffect = PLAYERSCRIPT->GetEffect();
	pEffect->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0.3f, 0.1f, 1.f, 1.f));
	pEffect->Animator2D()->Play(L"Concentrate");
	pEffect->Activate();
}

void CPlayerConcentrate_Start::Exit()
{
	PLAYERSCRIPT->DeactiveCombo();
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	// off effect
	PLAYERSCRIPT->GetEffect()->Deactivate();
}