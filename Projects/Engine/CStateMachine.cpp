#include "pch.h"
#include "CStateMachine.h"

CStateMachine::CStateMachine()
	: CComponent(COMPONENT_TYPE::STATEMACHINE)
{
}

CStateMachine::~CStateMachine()
{
}

void CStateMachine::begin()
{
	// ���� FSM ������ Instance �� �����صд�.
	if (m_FSM_Origin.Get())
	{
		m_FSM_Inst = m_FSM_Origin->GetFSMIstance();
		m_FSM_Inst->SetStateMachine(this);
		//m_FSM_Inst->m_Blackboard_OBJ = &m_Blackboard;
	}
}

void CStateMachine::finaltick()
{
	if (m_FSM_Inst.Get())
		m_FSM_Inst->finaltick();
}

void CStateMachine::SetFSM(Ptr<CFSM> _FSM)
{
	// @TODO : �̹� instance ���� ��? �������� ����
	m_FSM_Origin = _FSM;
	m_FSM_Inst = nullptr;
}

void CStateMachine::AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _pData)
{
	m_Blackboard.AddBlackboardData(_strKey, _Type, _pData);
}

Ptr<CFSM> CStateMachine::GetFSM()
{
	if (not m_FSM_Origin.Get())
	{
		MessageBoxA(nullptr, "There is No Original FSM", "Failed to return FSM Instance", MB_OK);
		return nullptr;
	}

	if (not m_FSM_Inst.Get())
	{
		//return m_FSM_Origin;
		m_FSM_Inst = m_FSM_Origin->GetFSMIstance();
	}

	return m_FSM_Inst;
};

void* CStateMachine::GetBlackboardData(const wstring& _strKey)
{
	return m_Blackboard.GetBlackboardData(_strKey);
}

void CStateMachine::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_FSM_Origin, _File);
}

void CStateMachine::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_FSM_Origin, _File);
}