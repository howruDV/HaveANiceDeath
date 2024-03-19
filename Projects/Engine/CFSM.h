#pragma once
#include "CAsset.h"
#include "CState.h"
#include "CBlackboard.h"

// =======================================
// CFSM: Finite State Machine ���� ���� �ӽ�, AI Asset
// =======================================
// - ����ȭ: ����� �ΰ������� �����ϴ� ���Ͱ� ����
// - ������Ʈ�� ����ų �� ����

class CStateMachine;

class CFSM :
    public CAsset
{
private:
    CFSM*                           m_Master;       // ���� FSM
    CStateMachine*                  m_StateMachie;  // FSM�� ����ϴ� StateMachine

    unordered_map<wstring, CState*> m_mapState;     // FSM�� ���� �� �ִ� ����
    CState*                         m_CurState;     // FSM�� ���� ����
    CBlackboard*                    m_Blackboard;   // ����� FSM ���� (��������: ������Ʈ)

public:
    void SetState(const wstring& _strState);
    void SetStateMachine(CStateMachine* _SM) { if (m_Master) { m_StateMachie = _SM; } }
    void AddState(const wstring& _StateName, CState* _State);
    void ChangeState(const wstring& _strStateName);
    CState* FindState(const wstring& _StateName);
    CFSM* GetFSMIstance();
    CStateMachine* GetStateMachine() { return m_StateMachie; }

public:
    void finaltick();

public:
    CLONE_DISABLE(CFSM);
    CFSM(bool _bEngine = false);
    ~CFSM();
};

