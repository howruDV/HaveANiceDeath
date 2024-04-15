#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BLOOMSCRIPT,
	CAMCTRLSCRIPT,
	CORNERTRIGGERSCRIPT,
	ELEVATORSCRIPT,
	GAMEMGR,
	GAUGESCRIPT,
	HITBOXSCRIPT,
	INVENMGR,
	KOTSPEAR_PROJSCRIPT,
	MISSILESCRIPT_TEST,
	MONSTERSCRIPT,
	MONSTERSCRIPT_TEST,
	PARALLELSCRIPT,
	PLAYERMGR,
	PLAYERSCRIPT,
	PLAYERSCRIPT_TEST,
	PROGRESS_PLAYERHP,
	PROGRESS_PLAYERHP_ACTIVE,
	PROGRESS_PLAYERMP,
	ROTATESCRIPT,
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
