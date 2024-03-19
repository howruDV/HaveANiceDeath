#pragma once
#include "CComputeShader.h"
#include "CTexture.h"
// =======================================
// CSetColorShader: Compute Shader 테스트용
// =======================================

class CSetColorShader :
    public CComputeShader
{
private:
    Ptr<CTexture>   m_TargetTex;
    Vec4            m_Color;

public:
    virtual int UpdatePipeline() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    void SetTargetTexture(Ptr<CTexture> _tex) { m_TargetTex = _tex; }
    void SetColor(Vec3 _Color) { m_Color = _Color; }

public:
    CSetColorShader();
    ~CSetColorShader();
};

