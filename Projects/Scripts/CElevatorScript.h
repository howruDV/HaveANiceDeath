#pragma once
#include <Engine/CScript.h>

class CElevatorScript :
    public CScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(CElevatorScript);
    CElevatorScript();
    ~CElevatorScript();
};