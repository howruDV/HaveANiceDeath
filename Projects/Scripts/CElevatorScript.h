#pragma once
#include <Engine\CScript.h>
#include <Engine\CSound.h>
#include <deque>

class CElevatorScript :
    public CScript
{
private:
    deque<Ptr<CSound>> m_vecSound;
    bool m_bStartOpen;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

    void Open();
    void Close();

public:
    CLONE(CElevatorScript);
    CElevatorScript();
    CElevatorScript(const CElevatorScript& _Origin);
    ~CElevatorScript();
};