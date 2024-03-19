#pragma once
#include "UI.h"
// =======================================
// UIComponent: Inspector의 Component가 상속하는 Parent UI
// =======================================

class UIInspectorPannel;

class UIComponent :
    public UI
{
private:
    CGameObject*        m_TargetObject;
    UIInspectorPannel*  m_pInspector;
    COMPONENT_TYPE      m_Type;
    string              m_ComponentTitle;

public:
    virtual void render() override;
    virtual void render_update() override;

public:
    void SetInspector(UIInspectorPannel* _pInspector) { m_pInspector = _pInspector; }
    void SetTitle(const string& _title) { m_ComponentTitle = _title; }
    void SetTargetObject(CGameObject* _Target);
    UIInspectorPannel* GetInspector() { return m_pInspector; }
    COMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetTargetObject() { return m_TargetObject; }

public:
    UIComponent(const string& _strName, const string& _ID, COMPONENT_TYPE _Type);
    ~UIComponent();
};

