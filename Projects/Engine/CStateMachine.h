#pragma once
#include "CComponent.h"
#include "CFSM.h"
#include "CBlackboard.h"

// =======================================
// CStateMachine: FSM Component
// =======================================
// - Component: FSM, �� ������ �ΰ������� ���� ����

class CStateMachine :
    public CComponent
{
private:
    Ptr<CFSM>       m_FSM;          // ������ �ΰ����� ����
    Ptr<CFSM>       m_FSM_Inst;     // ������ �ΰ����� Instance
    CBlackboard     m_Blackboard;   // obj ���� ���

public:
    void SetFSM(Ptr<CFSM> _FSM);
    void AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _pData);
    Ptr<CFSM> GetFSM(){
        if (m_FSM_Inst.Get())
            return m_FSM_Inst;
        else
            return m_FSM;  // @TODO: Instance �ȸ����?
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

