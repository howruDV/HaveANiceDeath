#pragma once
#include "UIComponent.h"
class UIParticleSystem :
    public UIComponent
{
public:
    virtual void render_update() override;
    void SelectTexture(DWORD_PTR _dwData);

public:
    UIParticleSystem();
    ~UIParticleSystem();
};

