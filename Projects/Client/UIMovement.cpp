#include "pch.h"
#include "UIMovement.h"
#include "func_ImGUI.h"

#include <Engine/CMovement.h>

UIMovement::UIMovement()
	: UIComponent("Movement", "##Movement", COMPONENT_TYPE::MOVEMENT)
{
}

UIMovement::~UIMovement()
{
}

void UIMovement::render_update()
{
	if (not GetTargetObject())
		return;

	UIComponent::render_update();
	CMovement* pMovement = GetTargetObject()->Movement();

	// get object data
	//Vec3  vForce;
	//Vec3  vAccel;
	//Vec3  vVelocity;
	Vec3  vGravityForce = pMovement->GetGravityForce();
	float fMass = pMovement->GetMass();
	float fInitSpeed = pMovement->GetInitSpeed();
	float fMaxSpeed_Ground = pMovement->GetMaxSpeed_Ground();
	float fMaxSpeed_InAir = pMovement->GetMaxSpeed_InAir();
	float fFrictionScale = pMovement->GetFrictionScale();
	bool  bUseGravity = pMovement->IsUseGravity();
	bool  bGround = pMovement->IsGround();

	// draw
	TextBox("Mass    ");			ImGui::SameLine(); ImGui::DragFloat("##Mass", &fMass);
	TextBox("Initial Speed");		ImGui::SameLine(); ImGui::DragFloat("##InitialSpeed", &fInitSpeed);
	TextBox("Max Speed (Ground)");	ImGui::SameLine(); ImGui::DragFloat("##MaxSpeed_Ground", &fMaxSpeed_Ground);
	TextBox("Max Speed (InAir)");	ImGui::SameLine(); ImGui::DragFloat("##MaxSpeed_inAir", &fMaxSpeed_InAir);
	TextBox("Friction");			ImGui::SameLine(); ImGui::DragFloat("##FrictionScale", &fFrictionScale);
	TextBox("Gravity Force");		ImGui::SameLine(); ImGui::DragFloat3("##FrictionScale", vGravityForce);
	TextBox("Use Gravity");			ImGui::SameLine(); ImGui::Checkbox("##UseGravity", &bUseGravity);
	TextBox("Ground");				ImGui::SameLine(); ImGui::Checkbox("##IsGround", &bGround);

	// set obect data
	pMovement->SetGravityForce(vGravityForce);
	pMovement->SetMass(fMass);
	pMovement->SetInitSpeed(fInitSpeed);
	pMovement->SetMaxSpeed_Ground(fMaxSpeed_Ground);
	pMovement->SetMaxSpeed_InAir(fMaxSpeed_InAir);
	pMovement->SetFrictionScale(fFrictionScale);
	pMovement->UseGravity(bUseGravity);
	pMovement->SetGround(bGround);
}