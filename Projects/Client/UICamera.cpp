#include "pch.h"
#include "UICamera.h"
#include "func_ImGUI.h"
#include <Engine/CCamera.h>

UICamera::UICamera()
	: UIComponent("Camera##", "Camera", COMPONENT_TYPE::CAMERA)
{
}

UICamera::~UICamera()
{
}

void UICamera::render_update()
{
	if (not GetTargetObject())
		return;

	UIComponent::render_update();

	// get camera member vars
	int			item_currentType = (int)GetTargetObject()->Camera()->GetProjType();
	float		m_FOV = GetTargetObject()->Camera()->GetFOV();
	float		m_Width = GetTargetObject()->Camera()->GetWidth();
	float		m_Scale = GetTargetObject()->Camera()->GetScale();
	float		m_AspectRatio = GetTargetObject()->Camera()->GetAspectRadio();
	float		m_Far = GetTargetObject()->Camera()->GetFar();

	// render UI
	ImGui::SeparatorText("Common Setting");
	const char* items[] = { "Orthographic", "Perspective" };
	static ImGuiComboFlags flags = 0;
	TextBox("Projection"); ImGui::SameLine();
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
	TextBox("Far"); ImGui::SameLine(); ImGui::DragFloat("##Camera Far", &m_Far);

	ImGui::SeparatorText("Orthographic Setting");
	TextBox("Width"); ImGui::SameLine(); ImGui::DragFloat("##Camera Ortho Width", &m_Width);
	TextBox("Scale"); ImGui::SameLine(); ImGui::DragFloat("##Camera Ortho Scale", &m_Scale);

	ImGui::SeparatorText("Perspective Setting");
	TextBox("FOV"); ImGui::SameLine(); ImGui::DragFloat("##Camera Perspecitve FOV", &m_FOV);

	// set camera member vars
	GetTargetObject()->Camera()->SetProjType((PROJ_TYPE)item_currentType);
	GetTargetObject()->Camera()->SetFOV(m_FOV);
	GetTargetObject()->Camera()->SetWidth(m_Width);
	GetTargetObject()->Camera()->SetScale(m_Scale);
	GetTargetObject()->Camera()->SetAspectRadio(m_AspectRatio);
	GetTargetObject()->Camera()->SetFar(m_Far);
}