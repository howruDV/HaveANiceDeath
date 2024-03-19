#include "pch.h"
#include "UIAnimator2D.h"

UIAnimator2D::UIAnimator2D()
	: UIComponent("Animator2D", "##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
{
}

UIAnimator2D::~UIAnimator2D()
{
}

void UIAnimator2D::render_update()
{
	if (not GetTargetObject())
		return;

	UIComponent::render_update();
}
