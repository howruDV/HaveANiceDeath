#pragma once
#include "UIComponent.h"
class UICamera :
    public UIComponent
{
public:
    virtual void render_update() override;

public:
    UICamera();
    ~UICamera();
};

