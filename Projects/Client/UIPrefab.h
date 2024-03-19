#pragma once
#include "UIAsset.h"

class UIPrefab :
    public UIAsset
{


public:
    virtual void render_update() override;

public:
    UIPrefab();
    ~UIPrefab();
};

