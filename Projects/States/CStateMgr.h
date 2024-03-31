#pragma once

#include <vector>
#include <string>

enum STATE_TYPE
{
	IDLESTATE,
	PLAYERDASH,
	PLAYERIDLE,
	PLAYERIDLETORUN,
	PLAYERIDLEUTURN,
	PLAYERJUMPFALLING,
	PLAYERJUMPLANDING,
	PLAYERJUMPSTART,
	PLAYERRUN,
	PLAYERRUNTOIDLE,
	PLAYERRUNUTURN,
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
