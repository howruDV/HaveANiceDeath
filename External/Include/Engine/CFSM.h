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
    CFSM*                           m_Origin;       // 원본 FSM
    CStateMachine*                  m_StateMachine;  // FSM을 사용하는 StateMachine

    unordered_map<wstring, CState*> m_mapState;     // FSM이 가질 수 있는 상태
    CState*                         m_CurState;     // FSM의 현재 상태
    CBlackboard*                    m_Blackboard_OBJ;   // 기록한 FSM 정보 (실제소유: 컴포넌트)
    CBlackboard*                    m_Blackboard_FSM;   // 기록한 FSM 정보 (실제소유: 컴포넌트)

public:
    void finaltick();
    virtual int Save(const wstring& _strRelativePath);
    virtual int Load(const wstring& _strFilePath);

public:
    void SetStateMachine(CStateMachine* _SM) { if (m_Origin) { m_StateMachine = _SM; } }
    void AddState(const wstring& _StateName, CState* _State);
    //void SetState(const wstring& _strState) { m_CurState = FindState(_strState); }    // Enter로 진입 못하고 바로 시작하므로 터질 가능성 농후
    void ChangeState(const wstring& _strStateName);
    CState* FindState(const wstring& _StateName);
    CFSM* GetFSMIstance();
    CStateMachine* GetStateMachine() { return m_StateMachine; }
    CBlackboard* GetBlackboard_OBJ() { return m_Blackboard_OBJ; }
    CBlackboard* GetBlackboard_FSM() { return m_Blackboard_FSM; }

private:
    void ChangeState_proc(CState* _pNextState);


public:
    CLONE_DISABLE(CFSM);
    CFSM(CFSM* _Origin, bool _bEngine = false);
    ~CFSM();

    friend class CTaskMgr;
    friend class CStateMachine;
};