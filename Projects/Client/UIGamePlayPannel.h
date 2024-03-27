#pragma once
#include "UI.h"

class UIGamePlayPannel :
    public UI
{
private:
    ImVec2 BtnSize;
    Ptr<CTexture> m_PlayIcon[2];
    Ptr<CTexture> m_PauseIcon[2];
    Ptr<CTexture> m_StopIcon;

public:
    virtual void begin() override;
    virtual void render_update() override;

public:
    UIGamePlayPannel();
    ~UIGamePlayPannel();
};

