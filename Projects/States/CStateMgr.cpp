#include "pch.h"
#include "CStateMgr.h"

#include "CIdleState.h"
#include "CPlayerDash.h"
#include "CPlayerIdle.h"
#include "CPlayerIdleToRun.h"
#include "CPlayerIdleUTurn.h"
#include "CPlayerJumpFalling.h"
#include "CPlayerJumpLanding.h"
#include "CPlayerJumpStart.h"
#include "CPlayerRun.h"
#include "CPlayerRunToIdle.h"
#include "CPlayerRunUTurn.h"
#include "CTraceState.h"

void CStateMgr::GetStateInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CIdleState");
	_vec.push_back(L"CPlayerDash");
	_vec.push_back(L"CPlayerIdle");
	_vec.push_back(L"CPlayerIdleToRun");
	_vec.push_back(L"CPlayerIdleUTurn");
	_vec.push_back(L"CPlayerJumpFalling");
	_vec.push_back(L"CPlayerJumpLanding");
	_vec.push_back(L"CPlayerJumpStart");
	_vec.push_back(L"CPlayerRun");
	_vec.push_back(L"CPlayerRunToIdle");
	_vec.push_back(L"CPlayerRunUTurn");
	_vec.push_back(L"CTraceState");
}

CState* CStateMgr::GetState(const wstring& _strStateName)
{
	if (L"CIdleState" == _strStateName)
		return new CIdleState;
	if (L"CPlayerDash" == _strStateName)
		return new CPlayerDash;
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
	if (L"CPlayerRun" == _strStateName)
		return new CPlayerRun;
	if (L"CPlayerRunToIdle" == _strStateName)
		return new CPlayerRunToIdle;
	if (L"CPlayerRunUTurn" == _strStateName)
		return new CPlayerRunUTurn;
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
	case (UINT)STATE_TYPE::PLAYERDASH:
		return new CPlayerDash;
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
	case (UINT)STATE_TYPE::PLAYERRUN:
		return new CPlayerRun;
		break;
	case (UINT)STATE_TYPE::PLAYERRUNTOIDLE:
		return new CPlayerRunToIdle;
		break;
	case (UINT)STATE_TYPE::PLAYERRUNUTURN:
		return new CPlayerRunUTurn;
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

	case STATE_TYPE::PLAYERDASH:
		return L"CPlayerDash";
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

	case STATE_TYPE::PLAYERRUN:
		return L"CPlayerRun";
		break;

	case STATE_TYPE::PLAYERRUNTOIDLE:
		return L"CPlayerRunToIdle";
		break;

	case STATE_TYPE::PLAYERRUNUTURN:
		return L"CPlayerRunUTurn";
		break;

	case STATE_TYPE::TRACESTATE:
		return L"CTraceState";
		break;

	}
	return nullptr;
}