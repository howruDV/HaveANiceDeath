#include "pch.h"
#include "UIComponent.h"
#include "UIInspectorPannel.h"

UIComponent::UIComponent(const string& _strName, const string& _ID, COMPONENT_TYPE _Type)
	: UI(_strName, _ID)
	, m_pInspector(nullptr)
	, m_TargetObject(nullptr)
	, m_Type(_Type)
{
}

UIComponent::~UIComponent()
{
}

void UIComponent::render()
{
	if (!IsActivate())
		return;

	if (ImGui::CollapsingHeader(this->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		render_update();
		const vector<UI*> vecChild = GetChilds();
		for (size_t i = 0; i < vecChild.size(); ++i)
			vecChild[i]->render();

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
}

void UIComponent::render_update()
{
}

void UIComponent::SetTargetObject(CGameObject* _Target)
{
	m_TargetObject = _Target;

	// TargetObject가 없거나, TargetObject가 해당 컴포넌트를 보유하지 않는다면 비활성화
	if (!m_TargetObject)
		Deactivate();
	else
	{
		if (!m_TargetObject->GetComponent(m_Type))
			Deactivate();
		else
			Activate();
	}
}