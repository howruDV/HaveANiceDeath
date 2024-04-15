#pragma once
#include "CProgressBarScript.h"

class CProgress_PlayerHP :
    public CProgressBarScript
{
public:
    virtual void begin() override;

private:
    virtual float GetProgress() override;

public:
    CLONE(CProgress_PlayerHP);
    CProgress_PlayerHP();
    ~CProgress_PlayerHP();
};