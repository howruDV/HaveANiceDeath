#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BLOOMSCRIPT,
	CORNERTRIGGERSCRIPT,
	GAMEMGR,
	HITBOXSCRIPT,
	KOTBIGSCRIPT,
	KOTSPEAR_PROJSCRIPT,
	MISSILESCRIPT_TEST,
	MONSTERSCRIPT,
	MONSTERSCRIPT_TEST,
	PLAYERMGR,
	PLAYERSCRIPT,
	PLAYERSCRIPT_TEST,
	WALLSCRIPT,
	SCRIPT_END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
