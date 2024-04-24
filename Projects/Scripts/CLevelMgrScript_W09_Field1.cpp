#include "pch.h"
#include "CLevelMgrScript_W09_Field1.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"
#include "CGameMgr.h"

#include <Engine\CRenderMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>

CLevelMgrScript_W09_Field1::CLevelMgrScript_W09_Field1()
	: CScript(LEVELMGRSCRIPT_W09_FIELD1)
{
}

CLevelMgrScript_W09_Field1::~CLevelMgrScript_W09_Field1()
{
	GamePlayStatic::Stop2DBGM(L"sound\\field_time\\HAND World 8 WORLD V2 2.2.wav");
	GamePlayStatic::Stop2DSound(L"sound\\field_time\\Amb_Elt_W08_ClockMechanisms_Lp_01.wav");
}

void CLevelMgrScript_W09_Field1::begin()
{
	// sound
	GamePlayStatic::Play2DBGM(L"sound\\field_time\\HAND World 8 WORLD V2 2.2.wav", 0.3f);
	GamePlayStatic::Play2DSound(L"sound\\field_time\\Amb_Elt_W08_ClockMechanisms_Lp_01.wav", 0, 0.15f);

	// player
	CPlayerMgr::GetPlayer()->StateMachine()->GetFSM()->ChangeState(L"Idle");

	// UI
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (pLevel)
	{
		CGameObject* HUD = pLevel->FindObjectByName(L"HUD", 31);

		if (HUD)
		{
			Vec3 Pos = HUD->Transform()->GetRelativePos();
			Pos.x =	- CRenderMgr::GetInst()->GetWinResol().x / 2.f + CGameMgr::GetHUDPosFromScreenLT().x;
			Pos.y =	CRenderMgr::GetInst()->GetWinResol().y / 2.f - CGameMgr::GetHUDPosFromScreenLT().y;

			HUD->Transform()->SetRelativePos(Pos);
		}
	}
}

void CLevelMgrScript_W09_Field1::tick()
{
}

Vec3 CLevelMgrScript_W09_Field1::CheckCamArea(Vec3 _Pos)
{
	Vec3 ret = _Pos;

	if (_Pos.x < -7200.f)
		ret.x = -7200.f;

	if (_Pos.x > 7120.f)
		ret.x = 7120.f;

	return ret;
}