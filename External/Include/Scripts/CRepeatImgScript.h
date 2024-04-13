#pragma once
#include <Engine/CScript.h>

class CRepeatImgScript :
    public CScript
{
private:
    int m_RepeatX;
    int m_RepeatY;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(CRepeatImgScript);
    CRepeatImgScript();
    CRepeatImgScript(const CRepeatImgScript& _Origin);
    ~CRepeatImgScript();
};