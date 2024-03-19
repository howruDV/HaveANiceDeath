#pragma once
#include "UIComponent.h"
class UICollider2D :
    public UIComponent
{
    virtual void render_update() override;

public:
    UICollider2D();
    ~UICollider2D();
};

