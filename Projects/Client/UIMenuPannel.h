#pragma once
#include "UI.h"
// =======================================
// UIMenuPannel: Menu bar
// =======================================

class UIMenuPannel :
    public UI
{
public:
    virtual void render() override;
    virtual void render_update() override;

    void File();
    void Tool();
    void Level();
    void GameObject();
    void Asset();

public:
    UIMenuPannel();
    ~UIMenuPannel();
};

