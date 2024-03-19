#pragma once
#include "UIComponent.h"
class UIParticleSystem :
    public UIComponent
{
public:
    virtual void render_update() override;

public:
    UIParticleSystem();
    ~UIParticleSystem();
};

