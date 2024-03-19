#pragma once
#include "CEntity.h"

// =======================================
// CBlackboard: 공유할 데이터 기록
// =======================================
// - blackboard는 실제로 state machine이 소유

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
    BB_DATA     Type;   // 저장 자료형
    void*       pData;  // 설명
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

