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
    CFSM*                           m_Origin;       // ���� FSM
    CStateMachine*                  m_StateMachie;  // FSM�� ����ϴ� StateMachine

    unordered_map<wstring, CState*> m_mapState;     // FSM�� ���� �� �ִ� ����
    CState*                         m_CurState;     // FSM�� ���� ����
    CBlackboard*                    m_Blackboard_OBJ;   // ����� FSM ���� (��������: ������Ʈ)
    CBlackboard*                    m_Blackboard_FSM;   // ����� FSM ���� (��������: ������Ʈ)

public:
    void SetStateMachine(CStateMachine* _SM) { if (m_Origin) { m_StateMachie = _SM; } }
    void AddState(const wstring& _StateName, CState* _State);
    void SetState(const wstring& _strState) {m_CurState = FindState(_strState); }
    void ChangeState(const wstring& _strStateName);
    void ChangeState_proc(CState* _pNextState);
    CState* FindState(const wstring& _StateName);
    CFSM* GetFSMIstance();
    CStateMachine* GetStateMachine() { return m_StateMachie; }
    CBlackboard* GetBlackboard_OBJ() { return m_Blackboard_OBJ; }
    CBlackboard* GetBlackboard_FSM() { return m_Blackboard_FSM; }

public:
    void finaltick();

public:
    CLONE_DISABLE(CFSM);
    CFSM(CFSM* _Origin, bool _bEngine = false);
    ~CFSM();
};