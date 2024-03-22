#include "pch.h"
#include "CScriptMgr.h"

#include "CMissileScript_Test.h"
#include "CMonsterScript_Test.h"
#include "CPlayerScript_Test.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CMissileScript_Test");
	_vec.push_back(L"CMonsterScript_Test");
	_vec.push_back(L"CPlayerScript_Test");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CMissileScript_Test" == _strScriptName)
		return new CMissileScript_Test;
	if (L"CMonsterScript_Test" == _strScriptName)
		return new CMonsterScript_Test;
	if (L"CPlayerScript_Test" == _strScriptName)
		return new CPlayerScript_Test;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT_TEST:
		return new CMissileScript_Test;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT_TEST:
		return new CMonsterScript_Test;
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
	case SCRIPT_TYPE::MISSILESCRIPT_TEST:
		return L"CMissileScript_Test";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT_TEST:
		return L"CMonsterScript_Test";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT_TEST:
		return L"CPlayerScript_Test";
		break;

	}
	return nullptr;
}