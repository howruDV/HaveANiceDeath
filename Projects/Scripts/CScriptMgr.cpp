#include "pch.h"
#include "CScriptMgr.h"

#include "CBloomScript.h"
#include "CCamCtrlScript.h"
#include "CCornerTriggerScript.h"
#include "CElevatorScript.h"
#include "CGameMgr.h"
#include "CGaugeScript.h"
#include "CHitboxScript.h"
#include "CInvenMgr.h"
#include "CKoTSpear_ProjScript.h"
#include "CMissileScript_Test.h"
#include "CMonsterScript.h"
#include "CMonsterScript_Test.h"
#include "CParallelScript.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"
#include "CPlayerScript_Test.h"
#include "CProgress_PlayerHP.h"
#include "CProgress_PlayerHP_Active.h"
#include "CProgress_PlayerMP.h"
#include "CRotateScript.h"
#include "CTitleMgr.h"
#include "CWallScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBloomScript");
	_vec.push_back(L"CCamCtrlScript");
	_vec.push_back(L"CCornerTriggerScript");
	_vec.push_back(L"CElevatorScript");
	_vec.push_back(L"CGameMgr");
	_vec.push_back(L"CGaugeScript");
	_vec.push_back(L"CHitboxScript");
	_vec.push_back(L"CInvenMgr");
	_vec.push_back(L"CKoTSpear_ProjScript");
	_vec.push_back(L"CMissileScript_Test");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMonsterScript_Test");
	_vec.push_back(L"CParallelScript");
	_vec.push_back(L"CPlayerMgr");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerScript_Test");
	_vec.push_back(L"CProgress_PlayerHP");
	_vec.push_back(L"CProgress_PlayerHP_Active");
	_vec.push_back(L"CProgress_PlayerMP");
	_vec.push_back(L"CRotateScript");
	_vec.push_back(L"CTitleMgr");
	_vec.push_back(L"CWallScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBloomScript" == _strScriptName)
		return new CBloomScript;
	if (L"CCamCtrlScript" == _strScriptName)
		return new CCamCtrlScript;
	if (L"CCornerTriggerScript" == _strScriptName)
		return new CCornerTriggerScript;
	if (L"CElevatorScript" == _strScriptName)
		return new CElevatorScript;
	if (L"CGameMgr" == _strScriptName)
		return new CGameMgr;
	if (L"CGaugeScript" == _strScriptName)
		return new CGaugeScript;
	if (L"CHitboxScript" == _strScriptName)
		return new CHitboxScript;
	if (L"CInvenMgr" == _strScriptName)
		return new CInvenMgr;
	if (L"CKoTSpear_ProjScript" == _strScriptName)
		return new CKoTSpear_ProjScript;
	if (L"CMissileScript_Test" == _strScriptName)
		return new CMissileScript_Test;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMonsterScript_Test" == _strScriptName)
		return new CMonsterScript_Test;
	if (L"CParallelScript" == _strScriptName)
		return new CParallelScript;
	if (L"CPlayerMgr" == _strScriptName)
		return new CPlayerMgr;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerScript_Test" == _strScriptName)
		return new CPlayerScript_Test;
	if (L"CProgress_PlayerHP" == _strScriptName)
		return new CProgress_PlayerHP;
	if (L"CProgress_PlayerHP_Active" == _strScriptName)
		return new CProgress_PlayerHP_Active;
	if (L"CProgress_PlayerMP" == _strScriptName)
		return new CProgress_PlayerMP;
	if (L"CRotateScript" == _strScriptName)
		return new CRotateScript;
	if (L"CTitleMgr" == _strScriptName)
		return new CTitleMgr;
	if (L"CWallScript" == _strScriptName)
		return new CWallScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BLOOMSCRIPT:
		return new CBloomScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMCTRLSCRIPT:
		return new CCamCtrlScript;
		break;
	case (UINT)SCRIPT_TYPE::CORNERTRIGGERSCRIPT:
		return new CCornerTriggerScript;
		break;
	case (UINT)SCRIPT_TYPE::ELEVATORSCRIPT:
		return new CElevatorScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMEMGR:
		return new CGameMgr;
		break;
	case (UINT)SCRIPT_TYPE::GAUGESCRIPT:
		return new CGaugeScript;
		break;
	case (UINT)SCRIPT_TYPE::HITBOXSCRIPT:
		return new CHitboxScript;
		break;
	case (UINT)SCRIPT_TYPE::INVENMGR:
		return new CInvenMgr;
		break;
	case (UINT)SCRIPT_TYPE::KOTSPEAR_PROJSCRIPT:
		return new CKoTSpear_ProjScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT_TEST:
		return new CMissileScript_Test;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT_TEST:
		return new CMonsterScript_Test;
		break;
	case (UINT)SCRIPT_TYPE::PARALLELSCRIPT:
		return new CParallelScript;
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
	case (UINT)SCRIPT_TYPE::PROGRESS_PLAYERHP:
		return new CProgress_PlayerHP;
		break;
	case (UINT)SCRIPT_TYPE::PROGRESS_PLAYERHP_ACTIVE:
		return new CProgress_PlayerHP_Active;
		break;
	case (UINT)SCRIPT_TYPE::PROGRESS_PLAYERMP:
		return new CProgress_PlayerMP;
		break;
	case (UINT)SCRIPT_TYPE::ROTATESCRIPT:
		return new CRotateScript;
		break;
	case (UINT)SCRIPT_TYPE::TITLEMGR:
		return new CTitleMgr;
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
	case SCRIPT_TYPE::BLOOMSCRIPT:
		return L"CBloomScript";
		break;

	case SCRIPT_TYPE::CAMCTRLSCRIPT:
		return L"CCamCtrlScript";
		break;

	case SCRIPT_TYPE::CORNERTRIGGERSCRIPT:
		return L"CCornerTriggerScript";
		break;

	case SCRIPT_TYPE::ELEVATORSCRIPT:
		return L"CElevatorScript";
		break;

	case SCRIPT_TYPE::GAMEMGR:
		return L"CGameMgr";
		break;

	case SCRIPT_TYPE::GAUGESCRIPT:
		return L"CGaugeScript";
		break;

	case SCRIPT_TYPE::HITBOXSCRIPT:
		return L"CHitboxScript";
		break;

	case SCRIPT_TYPE::INVENMGR:
		return L"CInvenMgr";
		break;

	case SCRIPT_TYPE::KOTSPEAR_PROJSCRIPT:
		return L"CKoTSpear_ProjScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT_TEST:
		return L"CMissileScript_Test";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT_TEST:
		return L"CMonsterScript_Test";
		break;

	case SCRIPT_TYPE::PARALLELSCRIPT:
		return L"CParallelScript";
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

	case SCRIPT_TYPE::PROGRESS_PLAYERHP:
		return L"CProgress_PlayerHP";
		break;

	case SCRIPT_TYPE::PROGRESS_PLAYERHP_ACTIVE:
		return L"CProgress_PlayerHP_Active";
		break;

	case SCRIPT_TYPE::PROGRESS_PLAYERMP:
		return L"CProgress_PlayerMP";
		break;

	case SCRIPT_TYPE::ROTATESCRIPT:
		return L"CRotateScript";
		break;

	case SCRIPT_TYPE::TITLEMGR:
		return L"CTitleMgr";
		break;

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"CWallScript";
		break;

	}
	return nullptr;
}