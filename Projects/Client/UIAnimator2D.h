#pragma once
#include "UIComponent.h"
class UIAnimator2D :
    public UIComponent
{
    virtual void render_update() override;

public:
    UIAnimator2D();
    ~UIAnimator2D();
};

