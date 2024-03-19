#pragma once
#include "CComponent.h"
// =======================================
// CLight2D: 광원이 상속하는 parent class
// =======================================

class CLight2D :
    public CComponent
{
private:
    FLightInfo  m_Info;

public:
    virtual void finaltick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    const FLightInfo& GetLightInfo() { return m_Info; }

    void SetLightType(LIGHT_TYPE _type) { m_Info.LightType = (int)_type; }
    void SetLightColor(Vec3 _vColor) { m_Info.vColor = _vColor; }
    void SetSpecular(Vec3 _vSpec) { m_Info.vSpecular = _vSpec; }
    void SetAmbient(Vec3 _vAmb) { m_Info.vAmbient = _vAmb; }
    void SetRadius(float _fRadius) { m_Info.fRadius = _fRadius; }
    void SetAngle(float _fAngle) { m_Info.fAngle = _fAngle; }

    LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)m_Info.LightType; }
    Vec4 GetLightColor() { return m_Info.vColor; }
    Vec4 GetSpecular() { return m_Info.vSpecular; }
    Vec4 GetAmbient() { return m_Info.vAmbient; }
    float GetRadius() { return m_Info.fRadius; }
    float GetAngle() { return m_Info.fAngle; }

public:
    CLONE(CLight2D);
    CLight2D();
    ~CLight2D();
};

