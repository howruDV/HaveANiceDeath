#pragma once
#include "UI.h"
#include "Engine/CGameObject.h"
#include "Engine/CAsset.h"
// =======================================
// InspectorPannel: gameobject�� ������ component ������ ���� UI
// =======================================
// - GameObject: component, script
// - Asset:

class UIComponent;
class UIAsset;
class UIScript;
class ObjectController;

class UIInspectorPannel :
    public UI
{
private:
    ObjectController*   m_ObjController;
    CGameObject*        m_TargetObject;
    Ptr<CAsset>         m_TargetAsset;
    UIComponent*        m_arrCompUI[(UINT)COMPONENT_TYPE::END];
    UIAsset*            m_arrAssetUI[(UINT)ASSET_TYPE::END];
    vector<UIScript*>   m_vecScriptUI;

public:
    virtual void tick() override;
    virtual void render_update() override;

private:
    void CreateComponentUI();
    void CreateAssetUI();
    void DrawLayerUI();
    void ResizeScriptVec(UINT _Size);

public:
    void SetTargetObject(CGameObject* _Object);
    void SetTargetAsset(Ptr<CAsset> _Asset);
    CGameObject* GetTargetObject() { return m_TargetObject; }
    ObjectController* GetObjController() { return m_ObjController; }

public:
    UIInspectorPannel();
    ~UIInspectorPannel();
};

