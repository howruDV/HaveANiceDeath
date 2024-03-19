#include "pch.h"
#include "UIParticleSystem.h"

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
}