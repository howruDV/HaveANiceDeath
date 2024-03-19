#pragma once
#include "UIComponent.h"
// =======================================
// UITransform: transform component ¿¬°á
// =======================================

class UITransform :
    public UIComponent
{
public:
    virtual void render_update() override;

public:
    UITransform();
    ~UITransform();
};

