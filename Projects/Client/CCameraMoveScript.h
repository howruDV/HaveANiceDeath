#pragma once
#include <Engine/CScript.h>

// =======================================
// CCameraMoveScript: Camera 이동 기능 구현
// =======================================

class CCameraMoveScript :
    public CScript
{
private:
    float m_CamSpeed;

public:
    virtual void tick();

    virtual void SaveToFile(FILE* _File) override {};
    virtual void LoadFromFile(FILE* _File) override {};

private:
    void MoveOrthographic();
    void MovePerspective();

public:
    CLONE(CCameraMoveScript);
    CCameraMoveScript();
    ~CCameraMoveScript();
};