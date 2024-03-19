#pragma once
#include "UIComponent.h"
class UILight2D :
    public UIComponent
{
public:
    virtual void render_update() override;

public:
    UILight2D();
    ~UILight2D();
};

