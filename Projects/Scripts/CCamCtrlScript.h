#pragma once
#include <Engine/CScript.h>

class CGameObejct;

class CCamCtrlScript :
    public CScript
{
private:
    CGameObject* m_Target;
    Vec3 m_vPrevPos;
    Vec3 m_vMove;
    float m_fSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    void SetTarget(CGameObject* _Target) { m_Target = _Target; }

    Vec3 GetMove() { return m_vMove; }

private:
    Vec3 CheckCamArea(Vec3 _Pos);

public:
    CLONE(CCamCtrlScript);
    CCamCtrlScript();
    CCamCtrlScript(const CCamCtrlScript& _Origin);
    ~CCamCtrlScript();
};