#pragma once
#include "CProgressBarScript.h"

class CPlayerScript;

class CProgress_PlayerHP_Active :
    public CProgressBarScript
{
private:
    CPlayerScript* m_Player;

public:
    virtual void begin() override;

private:
    virtual float GetProgress() override;

public:
    CLONE(CProgress_PlayerHP_Active);
    CProgress_PlayerHP_Active();
    ~CProgress_PlayerHP_Active();
};