#include "pch.h"
#include "UICollider2D.h"
#include "func_ImGUI.h"
#include <Engine/CCollider2D.h>

UICollider2D::UICollider2D()
	: UIComponent("Collider2D", "##Collider2D", COMPONENT_TYPE::COLLIDER2D)
{
}

UICollider2D::~UICollider2D()
{
}

void UICollider2D::render_update()
{
	if (not GetTargetObject())
		return;

	UIComponent::render_update();

	// get collider member vars
	Vec3 vPos = Vec3(GetTargetObject()->Collider2D()->GetOffsetPos().x, GetTargetObject()->Collider2D()->GetOffsetPos().y, 0.f);
	Vec3 vScale = Vec3(GetTargetObject()->Collider2D()->GetOffsetScale().x, GetTargetObject()->Collider2D()->GetOffsetScale().y, 1.f);
	int iCollisionCount = GetTargetObject()->Collider2D()->GetCollisionCount();
	static int item_currentType = (int)GetTargetObject()->Collider2D()->GetColliderType();
	bool bAbsolute = GetTargetObject()->Collider2D()->IsAbsolute();

	ImGui::SeparatorText("Transform");
	TextBox("Offset");			ImGui::SameLine(); ImGui::DragFloat3("##Collider Offset Position", vPos);
	TextBox("Scale");			ImGui::SameLine(); ImGui::DragFloat3("##Collider Offset Scale", vScale);
	TextBox("Scale Absolute");	ImGui::SameLine(); ImGui::Checkbox("##Collider Offset Absolute", &bAbsolute);

	ImGui::SeparatorText("Collider");
	const char* items[] = { "Rectangle", "Circle" };
	static ImGuiComboFlags flags = 0;
	TextBox("Type");			ImGui::SameLine();
	if (ImGui::BeginCombo(" ", items[item_currentType], flags))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			const bool is_selected = (item_currentType == n);
			if (ImGui::Selectable(items[n], is_selected))
				item_currentType = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	TextBox("Collision");		ImGui::SameLine(); ImGui::Text(std::to_string(iCollisionCount).c_str());

	// set collider member vars
	GetTargetObject()->Collider2D()->SetOffsetPos(vPos);
	GetTargetObject()->Collider2D()->SetOffsetScale(vScale);
	GetTargetObject()->Collider2D()->SetAbsolute(bAbsolute);
	GetTargetObject()->Collider2D()->SetColliderType((COLLIDER2D_TYPE)item_currentType);
}