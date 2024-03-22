#include "pch.h"
#include "CMonsterScript_Test.h"
#include <Engine/CStateMachine.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CMonsterScript_Test::CMonsterScript_Test()
	: CScript(MONSTERSCRIPT_TEST)
	, m_DetectRange(400.f)
	, m_Speed(200.f)
{
}

CMonsterScript_Test::~CMonsterScript_Test()
{
}

void CMonsterScript_Test::begin()
{
	if (StateMachine())
	{
		StateMachine()->AddBlackboardData(L"DetectRange", BB_DATA::FLOAT, &m_DetectRange);
		StateMachine()->AddBlackboardData(L"Speed", BB_DATA::FLOAT, &m_Speed);

		// 플레이어를 찾아서 Object 타입으로 블랙보드에 기록한다.
		CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");
		if (pPlayer)
			StateMachine()->AddBlackboardData(L"TargetObject", BB_DATA::OBJECT, pPlayer);

		if (StateMachine()->GetFSM().Get())
		{
			StateMachine()->GetFSM()->SetState(L"IdleState");
		}
	}
}

void CMonsterScript_Test::tick()
{
}

void CMonsterScript_Test::SaveToFile(FILE* _File)
{
}

void CMonsterScript_Test::LoadFromFile(FILE* _File)
{
}

