#include "pch.h"
#include "CLevelMgrScript_W09_Field1.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

#include <Engine\CStateMachine.h>

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