#pragma once
#include "CEntity.h"

// =======================================
// CBlackboard: ������ ������ ���
// =======================================
// - blackboard�� ������ state machine�� ����

// -----------------------
// type def
// -----------------------
enum class BB_DATA
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    OBJECT,
};

struct FBlackboardData
{
    BB_DATA     Type;   // ���� �ڷ���
    void*       pData;  // ����
};



// -----------------------
// class
// -----------------------
class CBlackboard :
    public CEntity
{
private:
    unordered_map<wstring, FBlackboardData>   m_mapBBData;

public:
    void AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _Data);
    void* GetBlackboardData(const wstring& _strKey);

public:
    CLONE(CBlackboard);
    CBlackboard();
    CBlackboard(const CBlackboard& _board);
    ~CBlackboard();
};

