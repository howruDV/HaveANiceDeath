#include "pch.h"
#include "CScytheDissRest.h"

#include <Scripts\CPlayerMgr.h>
#include <Scripts\CPlayerScript.h>

CScytheDissRest::CScytheDissRest()
	: CState(SCYTHEDISSREST)
{
}

CScytheDissRest::~CScytheDissRest()
{
}

void CScytheDissRest::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CScytheDissRest::Enter()
{
	GamePlayStatic::Play2DSound(L"sound\\scythe\\diss\\PC_Atk_Flail_Rest_01.wav", 1, 0.25f);
	GetOwner()->Animator2D()->Play(L"ScytheDiss_Rest", false);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	CPlayerMgr::GetPlayerScript()->ResetRest();

	// 순서대로 스폰
}

void CScytheDissRest::Exit()
{
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}