#include "pch.h"
#include "UIInspectorPannel.h"
#include "UITransform.h"
#include "Engine/CGameObject.h"
#include "Engine/CTransform.h"

UITransform::UITransform()
	: UIComponent("Transform", "##Transform", COMPONENT_TYPE::TRANSFORM)
{
}

UITransform::~UITransform()
{
}

void UITransform::render_update()
{
	if (not GetTargetObject())
		return;

	UIComponent::render_update();

	CGameObject* pTarget = GetTargetObject();

	// get object data
	Vec3 vPos = pTarget->Transform()->GetRelativePos();
	Vec3 vScale = pTarget->Transform()->GetRelativeScale();
	Vec3 vRot = pTarget->Transform()->GetRelativeRotation();
	bool bAbsolute = pTarget->Transform()->IsAbsolute();
	vRot.ToDegree(); // radion to degree

	// draw
	ImGui::Text("Position"); ImGui::SameLine();  ImGui::DragFloat3("##Relative Position", vPos);
	ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::DragFloat3("##Relative Scale", vScale);
	ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::DragFloat3("##Relative Rotation", vRot);
	ImGui::Text("Scale Absolute"); ImGui::SameLine(); ImGui::Checkbox("##TransformAbsolute", &bAbsolute);

	// set obect data
	vRot.ToRadian();
	pTarget->Transform()->SetRelativePos(vPos);
	pTarget->Transform()->SetRelativeScale(vScale);
	pTarget->Transform()->SetRelativeRotation(vRot);
	pTarget->Transform()->SetAbsolute(bAbsolute);
	// Absolute °ª 
}
