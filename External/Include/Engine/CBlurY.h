#pragma once
#include "CComputeShader.h"
class CBlurY :
    public CComputeShader
{
private:
    Ptr<CTexture>   m_ResourceTex;
    Ptr<CTexture>   m_TargetTex;

public:
    void SetResourceTex(Ptr<CTexture> _Tex) { m_ResourceTex = _Tex; }
    void SetTargetTexture(Ptr<CTexture> _Tex) { m_TargetTex = _Tex; }

public:
    virtual int UpdatePipeline() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    CBlurY();
    ~CBlurY();
};

