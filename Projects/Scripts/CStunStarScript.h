#pragma once
#include <Engine/CScript.h>

class CStunStarScript :
    public CScript
{
private:
    int m_Idx;
    Vec3 m_OrgScale;
    float m_AccTime;
    bool m_bFadeIn;
    bool m_bFadeOut;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(CStunStarScript);
    CStunStarScript();
    CStunStarScript(const CStunStarScript& _Origin);
    ~CStunStarScript();
};