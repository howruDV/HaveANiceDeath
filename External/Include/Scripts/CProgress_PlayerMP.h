#pragma once
#include "CProgressBarScript.h"

class CPlayerScript;

class CProgress_PlayerMP :
    public CProgressBarScript
{
private:
    CPlayerScript* m_Player;

public:
    virtual void begin() override;

private:
    virtual float GetProgress() override;

public:
    CLONE(CProgress_PlayerMP);
    CProgress_PlayerMP();
    ~CProgress_PlayerMP();
};