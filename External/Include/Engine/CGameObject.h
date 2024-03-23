#pragma once
#include "CEntity.h"

#define GET_COMPONENT(Type, TYPE) class C##Type* Type(){return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE];}

// =======================================
// CGameObject: 모든 GameObject가 상속하는 parent class
// =======================================
class CComponent;
class CRenderComponent;
class CScript;

class CGameObject :
    public CEntity
{
private:
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];    // 배열로 관리
    CRenderComponent*       m_RenderCom;                            // render 관리
    vector<CScript*>        m_vecScript;
    vector<CGameObject*>    m_vecChild;
    CGameObject*            m_Parent;
    int                     m_iLayerIdx; // 소속된 Layer Index
    bool                    m_bDead;

public:
    void begin();
    void tick();
    virtual void finaltick();
    void render();

    int DisconnectWithParent();
    int DisconnectWithLayer();
    void Destroy();

public:
    void AddComponent(CComponent* _Component);
    void AddChild(CGameObject* _Child);

    CGameObject* GetParent() { return m_Parent; }
    bool IsDead() { return m_bDead; }
    vector<CGameObject*>& GetChild() { return m_vecChild; }
    const vector<CScript*>& GetScripts() { return m_vecScript; }
    int GetLayerIdx() { return m_iLayerIdx; }
    bool IsAncestor(CGameObject* _Other);

    // Component type의 반환: 매번 캐스팅해 쓰기 귀찮음
    // 대안1 macro:
    // 대안2 getter: 각 타입마다 함수로 만들어 반환형 만들어줌
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }
    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
    GET_COMPONENT(StateMachine, STATEMACHINE);
    GET_COMPONENT(Movement, MOVEMENT);

public:
    CLONE(CGameObject);
    CGameObject();
    CGameObject(const CGameObject& _OriginObject);
    ~CGameObject();

    friend class CLayer;
    friend class CTaskMgr;
};

