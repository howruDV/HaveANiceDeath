#include "pch.h"
#include "CPlayerConcentrate.h"

#include <Engine\CTimeMgr.h>

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts\CGameMgr.h>
#include <Scripts\CCamCtrlScript.h>

CPlayerConcentrate::CPlayerConcentrate()
	: CState(PLAYERCONCENTRATE)
{
}

CPlayerConcentrate::~CPlayerConcentrate()
{
}

void CPlayerConcentrate::finaltick()
{	
	// change state
	if (KEY_TAP(KEY::LSHIFT) && PLAYERSCRIPT->CanDash())
	{
		ChangeState(L"Dash");
		return;
	}

	if (KEY_RELEASED(KEY::LBTN) || KEY_NONE(KEY::LBTN))
		ChangeState(L"PowerUp");
}

void CPlayerConcentrate::Enter()
{
	GetOwner()->Animator2D()->Play(L"Concentrate", true);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	// camera
	FCamEffect Shake{};
	Shake.Type = CAMEFFECT_TYPE::SHAKE;
	Shake.fPlayTime = -1.f;
	Shake.fAccTime = 0.f;
	Shake.fVar = 5.f;

	CGameMgr::GetMainCamera()->GetScriptByType<CCamCtrlScript>()->PushEffect(Shake);

	// play effect
	CGameObject* pEffect = PLAYERSCRIPT->GetEffect();
	pEffect->Animator2D()->Play(L"Concentrate");
	pEffect->Activate();
}

void CPlayerConcentrate::Exit()
{
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	CGameMgr::GetMainCamera()->GetScriptByType<CCamCtrlScript>()->PopEffect();

	// off effect
	PLAYERSCRIPT->GetEffect()->Deactivate();
}