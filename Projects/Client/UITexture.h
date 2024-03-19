#pragma once
#include "UIAsset.h"
class UITexture :
    public UIAsset
{
public:
    virtual void render_update() override;

public:
    UITexture();
    ~UITexture();
};

