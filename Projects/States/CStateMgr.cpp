#include "pch.h"
#include "CStateMgr.h"

#include "CIdleState.h"
#include "CKoTBigAttack1.h"
#include "CKoTBigAttack2.h"
#include "CKoTFlyingAttack1.h"
#include "CKoTSpearAttack1.h"
#include "CKoTSpearAttack2.h"
#include "CKoTSpearAttack3.h"
#include "CMonsterAppear.h"
#include "CMonsterAppearWait.h"
#include "CMonsterDie.h"
#include "CMonsterHit.h"
#include "CMonsterIdle.h"
#include "CMonsterRun.h"
#include "CMonsterStun.h"
#include "CMonsterSurprised.h"
#include "CMonsterUTurn.h"
#include "CPlayerConcentrate.h"
#include "CPlayerConcentrate_Start.h"
#include "CPlayerDash.h"
#include "CPlayerDie.h"
#include "CPlayerHit.h"
#include "CPlayerIdle.h"
#include "CPlayerIdleToRun.h"
#include "CPlayerIdleUTurn.h"
#include "CPlayerJumpFalling.h"
#include "CPlayerJumpLanding.h"
#include "CPlayerJumpStart.h"
#include "CPlayerPowerUp.h"
#include "CPlayerRun.h"
#include "CPlayerRunToIdle.h"
#include "CPlayerRunUTurn.h"
#include "CScytheDissAerial.h"
#include "CScytheDissComboA.h"
#include "CScytheDissComboB.h"
#include "CScytheDissComboC.h"
#include "CScytheDissComboD.h"
#include "CScytheDissCrush.h"
#include "CScytheDissRest.h"
#include "CScytheDissSpecial.h"
#include "CScytheDissUp.h"
#include "CTraceState.h"

void CStateMgr::GetStateInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CIdleState");
	_vec.push_back(L"CKoTBigAttack1");
	_vec.push_back(L"CKoTBigAttack2");
	_vec.push_back(L"CKoTFlyingAttack1");
	_vec.push_back(L"CKoTSpearAttack1");
	_vec.push_back(L"CKoTSpearAttack2");
	_vec.push_back(L"CKoTSpearAttack3");
	_vec.push_back(L"CMonsterAppear");
	_vec.push_back(L"CMonsterAppearWait");
	_vec.push_back(L"CMonsterDie");
	_vec.push_back(L"CMonsterHit");
	_vec.push_back(L"CMonsterIdle");
	_vec.push_back(L"CMonsterRun");
	_vec.push_back(L"CMonsterStun");
	_vec.push_back(L"CMonsterSurprised");
	_vec.push_back(L"CMonsterUTurn");
	_vec.push_back(L"CPlayerConcentrate");
	_vec.push_back(L"CPlayerConcentrate_Start");
	_vec.push_back(L"CPlayerDash");
	_vec.push_back(L"CPlayerDie");
	_vec.push_back(L"CPlayerHit");
	_vec.push_back(L"CPlayerIdle");
	_vec.push_back(L"CPlayerIdleToRun");
	_vec.push_back(L"CPlayerIdleUTurn");
	_vec.push_back(L"CPlayerJumpFalling");
	_vec.push_back(L"CPlayerJumpLanding");
	_vec.push_back(L"CPlayerJumpStart");
	_vec.push_back(L"CPlayerPowerUp");
	_vec.push_back(L"CPlayerRun");
	_vec.push_back(L"CPlayerRunToIdle");
	_vec.push_back(L"CPlayerRunUTurn");
	_vec.push_back(L"CScytheDissAerial");
	_vec.push_back(L"CScytheDissComboA");
	_vec.push_back(L"CScytheDissComboB");
	_vec.push_back(L"CScytheDissComboC");
	_vec.push_back(L"CScytheDissComboD");
	_vec.push_back(L"CScytheDissCrush");
	_vec.push_back(L"CScytheDissRest");
	_vec.push_back(L"CScytheDissSpecial");
	_vec.push_back(L"CScytheDissUp");
	_vec.push_back(L"CTraceState");
}

CState* CStateMgr::GetState(const wstring& _strStateName)
{
	if (L"CIdleState" == _strStateName)
		return new CIdleState;
	if (L"CKoTBigAttack1" == _strStateName)
		return new CKoTBigAttack1;
	if (L"CKoTBigAttack2" == _strStateName)
		return new CKoTBigAttack2;
	if (L"CKoTFlyingAttack1" == _strStateName)
		return new CKoTFlyingAttack1;
	if (L"CKoTSpearAttack1" == _strStateName)
		return new CKoTSpearAttack1;
	if (L"CKoTSpearAttack2" == _strStateName)
		return new CKoTSpearAttack2;
	if (L"CKoTSpearAttack3" == _strStateName)
		return new CKoTSpearAttack3;
	if (L"CMonsterAppear" == _strStateName)
		return new CMonsterAppear;
	if (L"CMonsterAppearWait" == _strStateName)
		return new CMonsterAppearWait;
	if (L"CMonsterDie" == _strStateName)
		return new CMonsterDie;
	if (L"CMonsterHit" == _strStateName)
		return new CMonsterHit;
	if (L"CMonsterIdle" == _strStateName)
		return new CMonsterIdle;
	if (L"CMonsterRun" == _strStateName)
		return new CMonsterRun;
	if (L"CMonsterStun" == _strStateName)
		return new CMonsterStun;
	if (L"CMonsterSurprised" == _strStateName)
		return new CMonsterSurprised;
	if (L"CMonsterUTurn" == _strStateName)
		return new CMonsterUTurn;
	if (L"CPlayerConcentrate" == _strStateName)
		return new CPlayerConcentrate;
	if (L"CPlayerConcentrate_Start" == _strStateName)
		return new CPlayerConcentrate_Start;
	if (L"CPlayerDash" == _strStateName)
		return new CPlayerDash;
	if (L"CPlayerDie" == _strStateName)
		return new CPlayerDie;
	if (L"CPlayerHit" == _strStateName)
		return new CPlayerHit;
	if (L"CPlayerIdle" == _strStateName)
		return new CPlayerIdle;
	if (L"CPlayerIdleToRun" == _strStateName)
		return new CPlayerIdleToRun;
	if (L"CPlayerIdleUTurn" == _strStateName)
		return new CPlayerIdleUTurn;
	if (L"CPlayerJumpFalling" == _strStateName)
		return new CPlayerJumpFalling;
	if (L"CPlayerJumpLanding" == _strStateName)
		return new CPlayerJumpLanding;
	if (L"CPlayerJumpStart" == _strStateName)
		return new CPlayerJumpStart;
	if (L"CPlayerPowerUp" == _strStateName)
		return new CPlayerPowerUp;
	if (L"CPlayerRun" == _strStateName)
		return new CPlayerRun;
	if (L"CPlayerRunToIdle" == _strStateName)
		return new CPlayerRunToIdle;
	if (L"CPlayerRunUTurn" == _strStateName)
		return new CPlayerRunUTurn;
	if (L"CScytheDissAerial" == _strStateName)
		return new CScytheDissAerial;
	if (L"CScytheDissComboA" == _strStateName)
		return new CScytheDissComboA;
	if (L"CScytheDissComboB" == _strStateName)
		return new CScytheDissComboB;
	if (L"CScytheDissComboC" == _strStateName)
		return new CScytheDissComboC;
	if (L"CScytheDissComboD" == _strStateName)
		return new CScytheDissComboD;
	if (L"CScytheDissCrush" == _strStateName)
		return new CScytheDissCrush;
	if (L"CScytheDissRest" == _strStateName)
		return new CScytheDissRest;
	if (L"CScytheDissSpecial" == _strStateName)
		return new CScytheDissSpecial;
	if (L"CScytheDissUp" == _strStateName)
		return new CScytheDissUp;
	if (L"CTraceState" == _strStateName)
		return new CTraceState;
	return nullptr;
}

CState* CStateMgr::GetState(UINT _iStateType)
{
	switch (_iStateType)
	{
	case (UINT)STATE_TYPE::IDLESTATE:
		return new CIdleState;
		break;
	case (UINT)STATE_TYPE::KOTBIGATTACK1:
		return new CKoTBigAttack1;
		break;
	case (UINT)STATE_TYPE::KOTBIGATTACK2:
		return new CKoTBigAttack2;
		break;
	case (UINT)STATE_TYPE::KOTFLYINGATTACK1:
		return new CKoTFlyingAttack1;
		break;
	case (UINT)STATE_TYPE::KOTSPEARATTACK1:
		return new CKoTSpearAttack1;
		break;
	case (UINT)STATE_TYPE::KOTSPEARATTACK2:
		return new CKoTSpearAttack2;
		break;
	case (UINT)STATE_TYPE::KOTSPEARATTACK3:
		return new CKoTSpearAttack3;
		break;
	case (UINT)STATE_TYPE::MONSTERAPPEAR:
		return new CMonsterAppear;
		break;
	case (UINT)STATE_TYPE::MONSTERAPPEARWAIT:
		return new CMonsterAppearWait;
		break;
	case (UINT)STATE_TYPE::MONSTERDIE:
		return new CMonsterDie;
		break;
	case (UINT)STATE_TYPE::MONSTERHIT:
		return new CMonsterHit;
		break;
	case (UINT)STATE_TYPE::MONSTERIDLE:
		return new CMonsterIdle;
		break;
	case (UINT)STATE_TYPE::MONSTERRUN:
		return new CMonsterRun;
		break;
	case (UINT)STATE_TYPE::MONSTERSTUN:
		return new CMonsterStun;
		break;
	case (UINT)STATE_TYPE::MONSTERSURPRISED:
		return new CMonsterSurprised;
		break;
	case (UINT)STATE_TYPE::MONSTERUTURN:
		return new CMonsterUTurn;
		break;
	case (UINT)STATE_TYPE::PLAYERCONCENTRATE:
		return new CPlayerConcentrate;
		break;
	case (UINT)STATE_TYPE::PLAYERCONCENTRATE_START:
		return new CPlayerConcentrate_Start;
		break;
	case (UINT)STATE_TYPE::PLAYERDASH:
		return new CPlayerDash;
		break;
	case (UINT)STATE_TYPE::PLAYERDIE:
		return new CPlayerDie;
		break;
	case (UINT)STATE_TYPE::PLAYERHIT:
		return new CPlayerHit;
		break;
	case (UINT)STATE_TYPE::PLAYERIDLE:
		return new CPlayerIdle;
		break;
	case (UINT)STATE_TYPE::PLAYERIDLETORUN:
		return new CPlayerIdleToRun;
		break;
	case (UINT)STATE_TYPE::PLAYERIDLEUTURN:
		return new CPlayerIdleUTurn;
		break;
	case (UINT)STATE_TYPE::PLAYERJUMPFALLING:
		return new CPlayerJumpFalling;
		break;
	case (UINT)STATE_TYPE::PLAYERJUMPLANDING:
		return new CPlayerJumpLanding;
		break;
	case (UINT)STATE_TYPE::PLAYERJUMPSTART:
		return new CPlayerJumpStart;
		break;
	case (UINT)STATE_TYPE::PLAYERPOWERUP:
		return new CPlayerPowerUp;
		break;
	case (UINT)STATE_TYPE::PLAYERRUN:
		return new CPlayerRun;
		break;
	case (UINT)STATE_TYPE::PLAYERRUNTOIDLE:
		return new CPlayerRunToIdle;
		break;
	case (UINT)STATE_TYPE::PLAYERRUNUTURN:
		return new CPlayerRunUTurn;
		break;
	case (UINT)STATE_TYPE::SCYTHEDISSAERIAL:
		return new CScytheDissAerial;
		break;
	case (UINT)STATE_TYPE::SCYTHEDISSCOMBOA:
		return new CScytheDissComboA;
		break;
	case (UINT)STATE_TYPE::SCYTHEDISSCOMBOB:
		return new CScytheDissComboB;
		break;
	case (UINT)STATE_TYPE::SCYTHEDISSCOMBOC:
		return new CScytheDissComboC;
		break;
	case (UINT)STATE_TYPE::SCYTHEDISSCOMBOD:
		return new CScytheDissComboD;
		break;
	case (UINT)STATE_TYPE::SCYTHEDISSCRUSH:
		return new CScytheDissCrush;
		break;
	case (UINT)STATE_TYPE::SCYTHEDISSREST:
		return new CScytheDissRest;
		break;
	case (UINT)STATE_TYPE::SCYTHEDISSSPECIAL:
		return new CScytheDissSpecial;
		break;
	case (UINT)STATE_TYPE::SCYTHEDISSUP:
		return new CScytheDissUp;
		break;
	case (UINT)STATE_TYPE::TRACESTATE:
		return new CTraceState;
		break;
	}
	return nullptr;
}

const wchar_t * CStateMgr::GetStateName(CState * _pState)
{
	switch ((STATE_TYPE)_pState->GetStateType())
	{
	case STATE_TYPE::IDLESTATE:
		return L"CIdleState";
		break;

	case STATE_TYPE::KOTBIGATTACK1:
		return L"CKoTBigAttack1";
		break;

	case STATE_TYPE::KOTBIGATTACK2:
		return L"CKoTBigAttack2";
		break;

	case STATE_TYPE::KOTFLYINGATTACK1:
		return L"CKoTFlyingAttack1";
		break;

	case STATE_TYPE::KOTSPEARATTACK1:
		return L"CKoTSpearAttack1";
		break;

	case STATE_TYPE::KOTSPEARATTACK2:
		return L"CKoTSpearAttack2";
		break;

	case STATE_TYPE::KOTSPEARATTACK3:
		return L"CKoTSpearAttack3";
		break;

	case STATE_TYPE::MONSTERAPPEAR:
		return L"CMonsterAppear";
		break;

	case STATE_TYPE::MONSTERAPPEARWAIT:
		return L"CMonsterAppearWait";
		break;

	case STATE_TYPE::MONSTERDIE:
		return L"CMonsterDie";
		break;

	case STATE_TYPE::MONSTERHIT:
		return L"CMonsterHit";
		break;

	case STATE_TYPE::MONSTERIDLE:
		return L"CMonsterIdle";
		break;

	case STATE_TYPE::MONSTERRUN:
		return L"CMonsterRun";
		break;

	case STATE_TYPE::MONSTERSTUN:
		return L"CMonsterStun";
		break;

	case STATE_TYPE::MONSTERSURPRISED:
		return L"CMonsterSurprised";
		break;

	case STATE_TYPE::MONSTERUTURN:
		return L"CMonsterUTurn";
		break;

	case STATE_TYPE::PLAYERCONCENTRATE:
		return L"CPlayerConcentrate";
		break;

	case STATE_TYPE::PLAYERCONCENTRATE_START:
		return L"CPlayerConcentrate_Start";
		break;

	case STATE_TYPE::PLAYERDASH:
		return L"CPlayerDash";
		break;

	case STATE_TYPE::PLAYERDIE:
		return L"CPlayerDie";
		break;

	case STATE_TYPE::PLAYERHIT:
		return L"CPlayerHit";
		break;

	case STATE_TYPE::PLAYERIDLE:
		return L"CPlayerIdle";
		break;

	case STATE_TYPE::PLAYERIDLETORUN:
		return L"CPlayerIdleToRun";
		break;

	case STATE_TYPE::PLAYERIDLEUTURN:
		return L"CPlayerIdleUTurn";
		break;

	case STATE_TYPE::PLAYERJUMPFALLING:
		return L"CPlayerJumpFalling";
		break;

	case STATE_TYPE::PLAYERJUMPLANDING:
		return L"CPlayerJumpLanding";
		break;

	case STATE_TYPE::PLAYERJUMPSTART:
		return L"CPlayerJumpStart";
		break;

	case STATE_TYPE::PLAYERPOWERUP:
		return L"CPlayerPowerUp";
		break;

	case STATE_TYPE::PLAYERRUN:
		return L"CPlayerRun";
		break;

	case STATE_TYPE::PLAYERRUNTOIDLE:
		return L"CPlayerRunToIdle";
		break;

	case STATE_TYPE::PLAYERRUNUTURN:
		return L"CPlayerRunUTurn";
		break;

	case STATE_TYPE::SCYTHEDISSAERIAL:
		return L"CScytheDissAerial";
		break;

	case STATE_TYPE::SCYTHEDISSCOMBOA:
		return L"CScytheDissComboA";
		break;

	case STATE_TYPE::SCYTHEDISSCOMBOB:
		return L"CScytheDissComboB";
		break;

	case STATE_TYPE::SCYTHEDISSCOMBOC:
		return L"CScytheDissComboC";
		break;

	case STATE_TYPE::SCYTHEDISSCOMBOD:
		return L"CScytheDissComboD";
		break;

	case STATE_TYPE::SCYTHEDISSCRUSH:
		return L"CScytheDissCrush";
		break;

	case STATE_TYPE::SCYTHEDISSREST:
		return L"CScytheDissRest";
		break;

	case STATE_TYPE::SCYTHEDISSSPECIAL:
		return L"CScytheDissSpecial";
		break;

	case STATE_TYPE::SCYTHEDISSUP:
		return L"CScytheDissUp";
		break;

	case STATE_TYPE::TRACESTATE:
		return L"CTraceState";
		break;

	}
	return nullptr;
}