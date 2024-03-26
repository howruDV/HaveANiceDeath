#pragma once
#include "UIComponent.h"

class UIAnimator2D :
    public UIComponent
{
    virtual void render_update() override;

private:
    void AddAnimation();
    void DeleteAnimation(const wstring& _strAnimationName);

public:
    UIAnimator2D();
    ~UIAnimator2D();
};