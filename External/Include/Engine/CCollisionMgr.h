#pragma once
#include "singleton.h"
// =======================================
// CCollisionMgr: Collision 감지, Event 생성 및 관리 
// =======================================

union CollisionID
{
    struct
    {
        UINT LeftID;
        UINT RightID;
    };
    UINT_PTR id;
};

class CCollider2D;
class CGameObject;

class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
    SINGLE(CCollisionMgr)

private:
    UINT                    m_Matrix[LAYER_MAX];
    map<UINT_PTR, bool>     m_ColPrevInfo;  // 이전 프레임 충돌정보 기록

public:
    void begin();
    void tick();

public:
    void LayerCheck(UINT _left, UINT _right);
    void LayerCheck(const wstring& _LeftLayer, const wstring& _RightLayer);
    void Clear();

private:
    void CollisionBtwLayer(UINT _left, UINT _right);
    bool CollisionBtwCollider2D(CCollider2D* _pLeft, CCollider2D* _pRight);
    bool IsCollision(CCollider2D* _LeftCol, CCollider2D* _RightCol);
};