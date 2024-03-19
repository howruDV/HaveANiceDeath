#pragma once
#include "CAsset.h"
#include "CState.h"
#include "CBlackboard.h"

// =======================================
// CFSM: Finite State Machine 유한 상태 머신, AI Asset
// =======================================
// - 에셋화: 비슷한 인공지능을 공유하는 몬스터가 많음
// - 컴포넌트로 가리킬 수 있음

class CStateMachine;

class CFSM :
    public CAsset
{
private:
    CFSM*                           m_Master;       // 원본 FSM
    CStateMachine*                  m_StateMachie;  // FSM을 사용하는 StateMachine

    unordered_map<wstring, CState*> m_mapState;     // FSM이 가질 수 있는 상태
    CState*                         m_CurState;     // FSM의 현재 상태
    CBlackboard*                    m_Blackboard;   // 기록한 FSM 정보 (실제소유: 컴포넌트)

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

