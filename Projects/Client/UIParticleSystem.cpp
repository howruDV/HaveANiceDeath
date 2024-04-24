#include "pch.h"
#include "func_ImGUI.h"
#include "UIParticleSystem.h"
#include "UIParam.h"

#include <Engine\CParticleSystem.h>

UIParticleSystem::UIParticleSystem()
	: UIComponent("ParticleSystem##", "ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
{
}

UIParticleSystem::~UIParticleSystem()
{
}


void UIParticleSystem::render_update()
{
	if (not GetTargetObject())
		return;

	UIComponent::render_update();
	CParticleSystem* pParticleSystem = GetTargetObject()->ParticleSystem();

	// get object data
	int iParticleMax = pParticleSystem->GetParticleSpawnMax();
	Ptr<CTexture> pParticleTex = pParticleSystem->GetParticleTex();
	string strTexKey = "";
	if (pParticleTex != nullptr)
		strTexKey = WstrToStr(pParticleTex->GetKey());

	FParticleModule Module = pParticleSystem->GetModule();
	bool ModuleCheck[(UINT)PARTICLE_MODULE::END] = { 0, };
	for (int i = 0; i < (UINT)PARTICLE_MODULE::END; ++i)
	{
		if (Module.arrModuleCheck[i])
			ModuleCheck[i] = true;
	}

	// draw
	TextBox("Spawn Max");			ImGui::SameLine(); ImGui::DragInt("##SpawnMax", &iParticleMax);
	UIParam::Param_TEXTURE(pParticleTex, "Particle Texture", this, (DELEGATE_1)&UIParticleSystem::SelectTexture);
	
	// Module - Spawn
	ImGui::SeparatorText("Module: Spawn");
	TextBox("Use Spawn");		ImGui::SameLine(); ImGui::Checkbox("##Module_Spawn", &ModuleCheck[0]);

	if (ImGui::TreeNode("Spawn Setting##SpawnDetail"))
	{
		TextBox("Spawn Color");			ImGui::SameLine(); ImGui::DragFloat4("##Spawn_SpawnColor", Module.vSpawnColor);
		TextBox("Spawn Scale (Min)");	ImGui::SameLine(); ImGui::DragFloat4("##Spawn_SpawnScaleMin", Module.vSpawnScaleMin);
		TextBox("Spawn Scale (Max)");	ImGui::SameLine(); ImGui::DragFloat4("##Spawn_SpawnScaleMax", Module.vSpawnScaleMax);
		TextBox("Life (Min)");			ImGui::SameLine(); ImGui::DragFloat("##Spawn_LifeMin", &Module.LifeMin);
		TextBox("Life (Max)");			ImGui::SameLine(); ImGui::DragFloat("##Spawn_LifeMax", &Module.LifeMax);
		TextBox("Spawn Rate");			ImGui::SameLine(); ImGui::DragFloat("##Spawn_SpawnLate", &Module.MassMin);
		TextBox("Spawn Type");			ImGui::SameLine(); ImGui::DragFloat("##Spawn_SpawnType", &Module.MassMax);
		TextBox("Spawn Shape (0: Sphere, 1: Box)");			ImGui::SameLine(); ImGui::DragInt("##Spawn_SpawnShape", &Module.SpawnShape);
		TextBox("Sphere Radius"); ImGui::SameLine(); ImGui::DragFloat("##Spawn_SpawnShapeRadius", &Module.Radius);
		TextBox("Box Scale");	ImGui::SameLine(); ImGui::DragFloat4("##Spawn_SpawnShapeScale", Module.vSpawnBoxScale);

		ImGui::TreePop();
	}

	// Module - Drag
	ImGui::SeparatorText("Module: Drag");
	TextBox("Use Drag");		ImGui::SameLine(); ImGui::Checkbox("##Module_Drag", &ModuleCheck[1]);

	if (ImGui::TreeNode("Drag Setting##DragDetail"))
	{
		TextBox("Drag Time");			ImGui::SameLine(); ImGui::DragFloat("##DragTime", &Module.DragTime);

		ImGui::TreePop();
	}

	// Module - Scale
	ImGui::SeparatorText("Module: Scale");
	TextBox("Use Scale");		ImGui::SameLine(); ImGui::Checkbox("##Module_Scale", &ModuleCheck[2]);

	if (ImGui::TreeNode("Scale Setting##ScaleDetail"))
	{
		TextBox("Scale Ratio");			ImGui::SameLine(); ImGui::DragFloat4("##ScaleRatio", Module.vScaleRatio);

		ImGui::TreePop();
	}

	// Module - Velocity
	ImGui::SeparatorText("Module: Add Velocity");
	TextBox("Use Add Velocity");		ImGui::SameLine(); ImGui::Checkbox("##Module_AddVelocity", &ModuleCheck[3]);

	if (ImGui::TreeNode("Velocity Setting##VelocityDetail"))
	{
		TextBox("Add Velocity Type ");	ImGui::SetItemTooltip("(0: from center, 1: to center, 2: fix direction)"); ImGui::SameLine(); ImGui::DragInt("##AddVelocityType", &Module.AddVelocityType);
		TextBox("Speed (Min)");			ImGui::SameLine(); ImGui::DragFloat("##AddVelocitySpeedMin", &Module.SpeedMin);
		TextBox("Speed (Max)");			ImGui::SameLine(); ImGui::DragFloat("##AddVelocitySpeedMax", &Module.SpeedMax);
		TextBox("Fixed Angle");			ImGui::SameLine(); ImGui::DragFloat("##AddVelocityFixedAngle", &Module.FixedAngle);
		TextBox("Fixed Direction");		ImGui::SameLine(); ImGui::DragFloat4("##AddVelocityFixedDirection", Module.vScaleRatio);

		ImGui::TreePop();
	}

	// Module - nosie Force
	ImGui::SeparatorText("Module: Noise Force");
	TextBox("Use Nosie Force");	ImGui::SameLine(); ImGui::Checkbox("##Module_NosieForce", &ModuleCheck[4]);

	if (ImGui::TreeNode("Nosie Force Setting##NosieForceDetail"))
	{
		TextBox("Scale");			ImGui::SameLine(); ImGui::DragFloat("##NosieForceScale", &Module.NosieForceScale);
		TextBox("Term");			ImGui::SameLine(); ImGui::DragFloat("##NosieForceTerm", &Module.NosieForceTerm);

		ImGui::TreePop();
	}

	// Module - Caculate Force
	ImGui::SeparatorText("Module: Caculate Force");
	TextBox("Use Caculate Force");	ImGui::SameLine(); ImGui::Checkbox("##Module_CaculateForce", &ModuleCheck[5]);

	bool bGravity = Module.Gravity;
	if (ImGui::TreeNode("Caculate Force Setting##CaculateForceDetail"))
	{
		TextBox("Gravity");			ImGui::SameLine(); ImGui::Checkbox("##CaculateForceScale", &bGravity);
		TextBox("Gravity Scale");	ImGui::SameLine(); ImGui::DragFloat3("##CaculateForceTerm", Module.GravityScale);

		ImGui::TreePop();
	}
	Module.Gravity = bGravity;

	// Module - Render
	ImGui::SeparatorText("Module: Render");
	TextBox("Use Render");	ImGui::SameLine(); ImGui::Checkbox("##Module_Render", &ModuleCheck[6]);

	bool bVelocityAlign = Module.VelocityAlignment;
	if (ImGui::TreeNode("Render Setting##RenderDetail"))
	{
		TextBox("Velocity Alignment");	ImGui::SameLine(); ImGui::Checkbox("##RenderVelocityAlignment", &bVelocityAlign);
		TextBox("Alpha Based Life");	ImGui::SetItemTooltip("(0: Off, 1: NormalizedAge, 2: Age)"); ImGui::SameLine(); ImGui::DragInt("##RenderAlphaBasedLife", &Module.AlphaBasedLife);
		TextBox("Alpha Max Age");		ImGui::SameLine(); ImGui::DragFloat("##RenderAlphaMaxAge", &Module.AlphaMaxAge);

		ImGui::TreePop();
	}
	Module.VelocityAlignment = bVelocityAlign;

	// set obect data
	for (int i=0; i<(UINT)PARTICLE_MODULE::END; ++i)
	{
		if (ModuleCheck[i])
			Module.arrModuleCheck[i] = true;
		else
			Module.arrModuleCheck[i] = false;
	}

	pParticleSystem->SetParticleSpawnMax(iParticleMax);
	pParticleSystem->SetModule(Module);
}

void UIParticleSystem::SelectTexture(DWORD_PTR _dwData)
{
	string strTex = (char*)_dwData;
	wstring strTexName = StrToWstr(strTex);

	CParticleSystem* pParticleSystem = GetTargetObject()->ParticleSystem();
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strTexName);

	pParticleSystem->SetParticleTex(pTex);
}