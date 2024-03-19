#pragma once
#include "UIComponent.h"
class UITileMap :
    public UIComponent
{
public:
    virtual void render_update() override;

public:
    UITileMap();
    ~UITileMap();
};

