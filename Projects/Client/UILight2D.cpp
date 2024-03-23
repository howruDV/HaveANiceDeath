#include "pch.h"
#include "UILight2D.h"
#include "func_ImGUI.h"
#include <Engine/CLight2D.h>

UILight2D::UILight2D()
	: UIComponent("Light2D", "##Light2D", COMPONENT_TYPE::LIGHT2D)
{
}

UILight2D::~UILight2D()
{
}

void UILight2D::render_update()
{
	if (not GetTargetObject())
		return;

	UIComponent::render_update();

	// get collider member vars
	int item_currentType = (int)GetTargetObject()->Light2D()->GetLightType();
	Vec4 vColor = GetTargetObject()->Light2D()->GetLightColor();
	Vec4 vSpecular = GetTargetObject()->Light2D()->GetSpecular();
	Vec4 vAmbient = GetTargetObject()->Light2D()->GetAmbient();
	float fRadius = GetTargetObject()->Light2D()->GetRadius();
	float fAngle = GetTargetObject()->Light2D()->GetAngle();

	// UI render
	const char* items[] = { "Directional", "Point", "Spot" };
	static ImGuiComboFlags flags = 0;
	TextBox("Type	 "); ImGui::SameLine();
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
	TextBox("Radius   "); ImGui::SameLine(); ImGui::DragFloat("##Collider Radius", &fRadius);
	TextBox("Angle    "); ImGui::SameLine(); ImGui::DragFloat("##Collider Angle", &fAngle);

	TextBox("Color    "); ImGui::SameLine();
	float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.5f;
	ImGui::SetNextItemWidth(w);
	ImGui::ColorPicker3("##Light Color", (float*)&vColor, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

	TextBox("Specular "); ImGui::SameLine();
	ImGui::SetNextItemWidth(w);
	ImGui::ColorPicker3("##Light Specular", (float*)&vSpecular, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

	TextBox("Ambient  "); ImGui::SameLine();
	ImGui::SetNextItemWidth(w);
	ImGui::ColorPicker3("##Light Ambient", (float*)&vAmbient, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

	// set collider member vars
	GetTargetObject()->Light2D()->SetLightType((LIGHT_TYPE)item_currentType);
	GetTargetObject()->Light2D()->SetLightColor(vColor);
	GetTargetObject()->Light2D()->SetSpecular(vSpecular);
	GetTargetObject()->Light2D()->SetAmbient(vAmbient);
	GetTargetObject()->Light2D()->SetRadius(fRadius);
	GetTargetObject()->Light2D()->SetAngle(fAngle);
}
