#pragma once

#include <vector>
#include <string>

enum STATE_TYPE
{
	IDLESTATE,
	PLAYERCONCENTRATE,
	PLAYERCONCENTRATE_START,
	PLAYERDASH,
	PLAYERIDLE,
	PLAYERIDLETORUN,
	PLAYERIDLEUTURN,
	PLAYERJUMPFALLING,
	PLAYERJUMPLANDING,
	PLAYERJUMPSTART,
	PLAYERPOWERUP,
	PLAYERRUN,
	PLAYERRUNTOIDLE,
	PLAYERRUNUTURN,
	SCYTHEDISSAERIAL,
	SCYTHEDISSCOMBOA,
	SCYTHEDISSCOMBOB,
	SCYTHEDISSCOMBOC,
	SCYTHEDISSCOMBOD,
	SCYTHEDISSCRUSH,
	SCYTHEDISSSPECIAL,
	SCYTHEDISSUP,
	TRACESTATE,
	STATE_END,
};

using namespace std;

class CState;

class CStateMgr
{
public:
	static void GetStateInfo(vector<wstring>& _vec);
	static CState * GetState(const wstring& _strStateName);
	static CState * GetState(UINT _iStateType);
	static const wchar_t * GetStateName(CState * _pState);
};
