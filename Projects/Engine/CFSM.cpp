#include "pch.h"
#include "CFSM.h"
#include "CTaskMgr.h"
#include "CStateMachine.h"

#include <States/CStateMgr.h>

CFSM::CFSM(CFSM* _Origin, bool _bEngine)
	: CAsset(ASSET_TYPE::FSM, _bEngine)
	, m_Origin(_Origin)
	//, m_Blackboard_OBJ(nullptr)
	, m_StateMachine(nullptr)
	, m_CurState(nullptr)
	, m_PrevState(nullptr)
{
	// origin�� ���
	if (not m_Origin)
		m_Blackboard_FSM = new CBlackboard();
	else
		m_Blackboard_FSM = _Origin->m_Blackboard_FSM;
}

CFSM::~CFSM()
{
	// origin�� ���
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

int CFSM::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	// State�� ���� ����
	size_t StateCount = m_mapState.size();
	fwrite(&StateCount, sizeof(size_t), 1, pFile);

	// State ����
	unordered_map<wstring, CState*>::iterator iter = m_mapState.begin();

	for (; iter != m_mapState.end(); ++iter)
	{
		// State Key ����
		SaveWString(iter->first, pFile);

		// State Type ����: enum �и��� ����
		//UINT StateType = iter->second->GetStateType();
		//fwrite(&StateType, sizeof(UINT), 1, pFile);
		wstring StateName = CStateMgr::GetStateName(iter->second);
		SaveWString(StateName, pFile);

		// State ����
		iter->second->SaveToFile(pFile);
	}

	fclose(pFile);

	return S_OK;
}

int CFSM::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	if (nullptr == pFile)
		return E_FAIL;

	// State ���� �ε�
	size_t StateCount = 0;
	fread(&StateCount, sizeof(size_t), 1, pFile);

	// ������Ʈ �ε�
	for (size_t i = 0; i < StateCount; ++i)
	{
		// ������Ʈ Ű �ε�
		wstring StateKey;
		LoadWString(StateKey, pFile);

		// ������Ʈ Ÿ�� �ε�
		//UINT StateType;
		//fread(&StateType, sizeof(UINT), 1, pFile);
		wstring StateName;
		LoadWString(StateName, pFile);

		// ������Ʈ ����
		CState* pState = CStateMgr::GetState(StateName);
		pState->LoadFromFile(pFile);

		pState->m_FSM = this;
		m_mapState.insert(make_pair(StateKey, pState));
	}

	fclose(pFile);

	return S_OK;
}

void CFSM::AddState(const wstring& _StateName, CState* _State)
{
	assert(!(FindState(_StateName)));

	_State->m_FSM = this;
	_State->SetName(_StateName);
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
	// @TODO : ���� State ������ �Ҽ��� ������?
	//if (_strState == m_CurState->GetName())
	//	return;

	// 1. next state check
	CState* pNextState = FindState(_strState);
	assert(pNextState);

	// 2. adjust task to taskMgr
	// Param1: Parent Object    |   Param2: Next State
	FTask pTask = {};
	pTask.Type = TASK_TYPE::CHANGE_STATE;
	pTask.Param_1 = (UINT_PTR)m_StateMachine->GetOwner();
	pTask.Param_2 = (UINT_PTR)pNextState;
	CTaskMgr::GetInst()->AddTask(pTask);
}

void CFSM::DeleteState(const wstring& _StateKey)
{
	CState* pState = FindState(_StateKey);

	if (pState == nullptr)
	{
		MessageBoxA(nullptr, "�ش� Ű�� ���� ������Ʈ�� �����ϴ�.", "Delete State Failed", MB_OK);
		return;
	}

	delete pState;
	m_mapState.erase(_StateKey);
}

void CFSM::ChangeState_proc(CState* _pNextState)
{
	if (m_CurState)
		m_CurState->Exit();

	m_PrevState = m_CurState;
	m_CurState = _pNextState;
	m_CurState->m_FSM = this;
	m_CurState->Enter();
}

CFSM* CFSM::GetFSMIstance()
{
	CFSM* pFSMInst = new CFSM(this, true);	// @TODO ������ �������� trueüũ
	
	pFSMInst->m_mapState = m_mapState;

	return pFSMInst;
}