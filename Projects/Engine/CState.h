#pragma once
#include "CEntity.h"

// =======================================
// CState: State�� ����ϴ� parent class, �ϳ��� ����
// =======================================
// - ����: FSM�� ����
// - ���� state ����: ����ؼ� client �ο���

class CFSM;
class CStateMachine;

class CState :
    public CEntity
{
private:
    CFSM* m_FSM;    // �� state�� ������ FSM (AI)

public:
    virtual void finaltick() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;

protected:
    void ChangeState(const wstring& _strStateName);
    void* GetBlackboardData(const wstring& _strKey);
    CFSM* GetFSM() { return m_FSM; }

public:
    CLONE_DISABLE(CState);

    CState();
    ~CState();

    friend class CFSM;
};
