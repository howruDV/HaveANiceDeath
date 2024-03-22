#include "pch.h"
#include "CFSM.h"
#include "CTaskMgr.h"
#include "CStateMachine.h"

CFSM::CFSM(CFSM* _Origin, bool _bEngine)
	: CAsset(ASSET_TYPE::FSM, _bEngine)
	, m_Origin(_Origin)
	, m_Blackboard_OBJ(nullptr)
	, m_StateMachie(nullptr)
	, m_CurState(nullptr)
{
	// origin인 경우
	if (not m_Origin)
		m_Blackboard_FSM = new CBlackboard();
}

CFSM::~CFSM()
{
	// origin인 경우
	if (!m_Origin)
	{
		Delete_Map(m_mapState);
		if (m_Blackboard_FSM)
			delete m_Blackboard_FSM;
	}
}

void CFSM::finaltick()
{
	if (m_CurState)
	{
		m_CurState->m_FSM = this;
		m_CurState->finaltick();
	}
}

void CFSM::AddState(const wstring& _StateName, CState* _State)
{
	assert(!(FindState(_StateName)));

	_State->m_FSM = this;
	m_mapState.insert(make_pair(_StateName, _State));
}

CState* CFSM::FindState(const wstring& _StateName)
{
	unordered_map<wstring, CState*>::iterator iter = m_mapState.find(_StateName);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

void CFSM::ChangeState(const wstring& _strState)
{
	// 1. next state check
	CState* pNextState = FindState(_strState);
	assert(pNextState);

	// 2. adjust task to taskMgr
	// Param1: Parent Object    |   Param2: Next State
	FTask pTask = {};
	pTask.Type = TASK_TYPE::CHANGE_STATE;
	pTask.Param_1 = (UINT_PTR)m_StateMachie->GetOwner();
	pTask.Param_2 = (UINT_PTR)pNextState;
	CTaskMgr::GetInst()->AddTask(pTask);
}

void CFSM::ChangeState_proc(CState* _pNextState)
{
	if (m_CurState)
		m_CurState->Exit();

	//m_CurState = FindState(_strStateName); assert(m_CurState);
	m_CurState = _pNextState;
	m_CurState->Enter();
}

CFSM* CFSM::GetFSMIstance()
{
	CFSM* pFSMInst = new CFSM(this);

	pFSMInst->m_mapState = m_mapState;
	pFSMInst->m_Blackboard_OBJ = nullptr;
	pFSMInst->m_CurState = nullptr;

	return pFSMInst;
}