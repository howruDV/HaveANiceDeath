#pragma once
#include "CEntity.h"

#include "CKeyMgr.h"
#include "CGameObject.h"
#include "CMovement.h"
#include "CAnimator2D.h"
#include "CMeshRender.h"

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
    CFSM*           m_FSM;    // �� state�� ������ FSM (AI)
    const UINT      m_StateType;

public:
    virtual void finaltick() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;

    virtual void SaveToFile(FILE* pFile) {};
    virtual void LoadFromFile(FILE* pFile) {};

public:
    UINT GetStateType() { return m_StateType; }
    CGameObject* GetOwner();

protected:
    void ChangeState(const wstring& _strStateName);
    void* GetBlackboardData(const wstring& _strKey);
    CFSM* GetFSM() { return m_FSM; }

public:
    CLONE_DISABLE(CState);
    CState(UINT StateType);
    ~CState();

    friend class CFSM;
};
