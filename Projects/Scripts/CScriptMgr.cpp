#include "pch.h"
#include "CScriptMgr.h"

#include "CCornerTriggerScript.h"
#include "CGameMgr.h"
#include "CHitboxScript.h"
#include "CKoTBigScript.h"
#include "CMissileScript_Test.h"
#include "CMonsterScript_Test.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"
#include "CPlayerScript_Test.h"
#include "CWallScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCornerTriggerScript");
	_vec.push_back(L"CGameMgr");
	_vec.push_back(L"CHitboxScript");
	_vec.push_back(L"CKoTBigScript");
	_vec.push_back(L"CMissileScript_Test");
	_vec.push_back(L"CMonsterScript_Test");
	_vec.push_back(L"CPlayerMgr");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerScript_Test");
	_vec.push_back(L"CWallScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCornerTriggerScript" == _strScriptName)
		return new CCornerTriggerScript;
	if (L"CGameMgr" == _strScriptName)
		return new CGameMgr;
	if (L"CHitboxScript" == _strScriptName)
		return new CHitboxScript;
	if (L"CKoTBigScript" == _strScriptName)
		return new CKoTBigScript;
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
	if (L"CWallScript" == _strScriptName)
		return new CWallScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CORNERTRIGGERSCRIPT:
		return new CCornerTriggerScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMEMGR:
		return new CGameMgr;
		break;
	case (UINT)SCRIPT_TYPE::HITBOXSCRIPT:
		return new CHitboxScript;
		break;
	case (UINT)SCRIPT_TYPE::KOTBIGSCRIPT:
		return new CKoTBigScript;
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
	case (UINT)SCRIPT_TYPE::WALLSCRIPT:
		return new CWallScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CORNERTRIGGERSCRIPT:
		return L"CCornerTriggerScript";
		break;

	case SCRIPT_TYPE::GAMEMGR:
		return L"CGameMgr";
		break;

	case SCRIPT_TYPE::HITBOXSCRIPT:
		return L"CHitboxScript";
		break;

	case SCRIPT_TYPE::KOTBIGSCRIPT:
		return L"CKoTBigScript";
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

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"CWallScript";
		break;

	}
	return nullptr;
}