#pragma once
#include "UIAsset.h"

class UISound :
    public UIAsset
{
private:
    Ptr<CSound> m_RecentSound;

public:
    virtual void render_update() override;

public:
    UISound();
    ~UISound();
};

