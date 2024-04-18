#include "pch.h"
#include "CPlayerHit.h"

#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CGameMgr.h>
#include <Scripts\CCamCtrlScript.h>

CPlayerHit::CPlayerHit()
	: CState(PLAYERHIT)
{
}

CPlayerHit::~CPlayerHit()
{
}

void CPlayerHit::finaltick()
{
	// turn 중이면 state 변경 보류
	if (GetOwner()->Animator2D()->IsPlaying())
		return;

	GetFSM()->SetGlobalState(false);
	ChangeState(L"Idle");
}

void CPlayerHit::Enter()
{
	// camera
	FCamEffect Shake{};
	Shake.Type = CAMEFFECT_TYPE::SHAKE;
	Shake.fPlayTime = 0.3f;
	Shake.fVar = 1.5f;
	CGameMgr::GetMainCamera()->GetScriptByType<CCamCtrlScript>()->PushEffect(Shake);
	
	// setting
	GetOwner()->Movement()->SetVelocity(Vec3());
	PLAYERSCRIPT->SetDirLock(true);
	GetFSM()->SetGlobalState(true);

	// anim
	GetOwner()->Animator2D()->Play(L"Hit", false);

	// sound
	wstring strName = L"sound\\player\\Onos_Player_Death_Hit_light_0";
	int rand = Random(1, 9);
	strName += std::to_wstring(rand) + L".wav";
	GamePlayStatic::Play2DSound(strName, 1, 0.2f);
}

void CPlayerHit::Exit()
{
	PLAYERSCRIPT->SetDirLock(false);
}