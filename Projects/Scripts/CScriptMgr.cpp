#include "pch.h"
#include "CScriptMgr.h"

#include "CGameMgr.h"
#include "CMissileScript_Test.h"
#include "CMonsterScript_Test.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"
#include "CPlayerScript_Test.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CGameMgr");
	_vec.push_back(L"CMissileScript_Test");
	_vec.push_back(L"CMonsterScript_Test");
	_vec.push_back(L"CPlayerMgr");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerScript_Test");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CGameMgr" == _strScriptName)
		return new CGameMgr;
	if (L"CMissileScript_Test" == _strScriptName)
		return new CMissileScript_Test;
	if (L"CMonsterScript_Test" == _strScriptName)
		return new CMonsterScript_Test;
	if (L"CPlayerMgr" == _strScriptName)
		return new CPlayerMgr;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerScript_Test" == _strScriptName)
		return new CPlayerScript_Test;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::GAMEMGR:
		return new CGameMgr;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT_TEST:
		return new CMissileScript_Test;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT_TEST:
		return new CMonsterScript_Test;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERMGR:
		return new CPlayerMgr;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT_TEST:
		return new CPlayerScript_Test;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::GAMEMGR:
		return L"CGameMgr";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT_TEST:
		return L"CMissileScript_Test";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT_TEST:
		return L"CMonsterScript_Test";
		break;

	case SCRIPT_TYPE::PLAYERMGR:
		return L"CPlayerMgr";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT_TEST:
		return L"CPlayerScript_Test";
		break;

	}
	return nullptr;
}