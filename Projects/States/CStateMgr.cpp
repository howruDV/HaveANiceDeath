#include "pch.h"
#include "CStateMgr.h"

#include "CIdleState.h"
#include "CPlayerIdle.h"
#include "CPlayerRun.h"
#include "CPlayerTurn.h"
#include "CTraceState.h"

void CStateMgr::GetStateInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CIdleState");
	_vec.push_back(L"CPlayerIdle");
	_vec.push_back(L"CPlayerRun");
	_vec.push_back(L"CPlayerTurn");
	_vec.push_back(L"CTraceState");
}

CState* CStateMgr::GetState(const wstring& _strStateName)
{
	if (L"CIdleState" == _strStateName)
		return new CIdleState;
	if (L"CPlayerIdle" == _strStateName)
		return new CPlayerIdle;
	if (L"CPlayerRun" == _strStateName)
		return new CPlayerRun;
	if (L"CPlayerTurn" == _strStateName)
		return new CPlayerTurn;
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
	case (UINT)STATE_TYPE::PLAYERIDLE:
		return new CPlayerIdle;
		break;
	case (UINT)STATE_TYPE::PLAYERRUN:
		return new CPlayerRun;
		break;
	case (UINT)STATE_TYPE::PLAYERTURN:
		return new CPlayerTurn;
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

	case STATE_TYPE::PLAYERIDLE:
		return L"CPlayerIdle";
		break;

	case STATE_TYPE::PLAYERRUN:
		return L"CPlayerRun";
		break;

	case STATE_TYPE::PLAYERTURN:
		return L"CPlayerTurn";
		break;

	case STATE_TYPE::TRACESTATE:
		return L"CTraceState";
		break;

	}
	return nullptr;
}