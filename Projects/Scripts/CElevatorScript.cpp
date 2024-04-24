#include "pch.h"
#include "CElevatorScript.h"
#include "CPlayerMgr.h"

#include "CGameMgr.h"
#include "CCamCtrlScript.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CTransform.h>
#include <Engine\CStateMachine.h>

CElevatorScript::CElevatorScript()
	: CScript(ELEVATORSCRIPT)
	, m_bCanClose(false)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "Can Close", &m_bCanClose);
}

CElevatorScript::CElevatorScript(const CElevatorScript& _Origin)
	: CScript(ELEVATORSCRIPT)
	, m_bCanClose(_Origin.m_bCanClose)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "Can Close", &m_bCanClose);
}

CElevatorScript::~CElevatorScript()
{
}

void CElevatorScript::begin()
{
	// material
	GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0.1f, 0.2f, 0.1f, 1.f));
	GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.8f);

	// state
	StateMachine()->AddBlackboardData(L"bCanClose", BB_DATA::INT, &m_bCanClose);

	if (!m_bCanClose)
	{
		StateMachine()->GetFSM()->ChangeState(L"Appear");
	}
	else
	{
		StateMachine()->GetFSM()->ChangeState(L"Wait");
	}
}

void CElevatorScript::tick()
{
}

void CElevatorScript::SaveToFile(FILE* _File)
{
	fwrite(&m_bCanClose, sizeof(bool), 1, _File);
}

void CElevatorScript::LoadFromFile(FILE* _File)
{
	fread(&m_bCanClose, sizeof(bool), 1, _File);
}