#pragma once
#include "UI.h"
#include "Engine/CGameObject.h"
#include "Engine/CAsset.h"
// =======================================
// InspectorPannel: gameobject가 가지는 component 연결해 띄우는 UI
// =======================================
// - GameObject: component, script
// - Asset:

class UIComponent;
class UIAsset;
class UIScript;

class UIInspectorPannel :
    public UI
{
private:
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
    void ResizeScriptVec(UINT _Size);

public:
    void SetTargetObject(CGameObject* _Object);
    void SetTargetAsset(Ptr<CAsset> _Asset);
    CGameObject* GetTargetObject() { return m_TargetObject; }

public:
    UIInspectorPannel();
    ~UIInspectorPannel();
};

