#pragma once
#include "UIComponent.h"
class UICamera :
    public UIComponent
{
public:
    virtual void render_update() override;
    void CheckLayer(DWORD_PTR _ptr);

public:
    UICamera();
    ~UICamera();
};

