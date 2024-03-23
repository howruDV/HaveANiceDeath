#pragma once
#include "UIComponent.h"

class UIMovement :
    public UIComponent
{
public:
    virtual void render_update() override;

public:
    UIMovement();
    ~UIMovement();
};

