#pragma once
#include "CComponent.h"
#include "CFSM.h"
#include "CBlackboard.h"

// =======================================
// CStateMachine: FSM Component
// =======================================
// - Component: FSM, 즉 적용할 인공지능을 선택 가능

class CStateMachine :
    public CComponent
{
private:
    Ptr<CFSM>       m_FSM;          // 선택한 인공지능 원본
    Ptr<CFSM>       m_FSM_Inst;     // 선택한 인공지능 Instance
    CBlackboard     m_Blackboard;   // obj 전용 기록

public:
    void SetFSM(Ptr<CFSM> _FSM);
    void AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _pData);
    Ptr<CFSM> GetFSM(){
        if (m_FSM_Inst.Get())
            return m_FSM_Inst;
        else
            return m_FSM;  // @TODO: Instance 안만들고?
    };
    void* GetBlackboardData(const wstring& _strKey);

public:
    virtual void begin() override;
    virtual void finaltick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CStateMachine);
    CStateMachine();

    ~CStateMachine();
};

