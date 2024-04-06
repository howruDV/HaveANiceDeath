#pragma once
#include "CComputeShader.h"
class CCombine :
    public CComputeShader
{
private:
    Ptr<CTexture>   m_RenderTargetCopyTex;
    Ptr<CTexture>   m_BloomTex;
    Ptr<CTexture>   m_RenderTarget;
    float           m_fStrength;

public:
    void SetRenderTargetCopyTex(Ptr<CTexture> _Tex) { m_RenderTargetCopyTex = _Tex; }
    void SetBloomTex(Ptr<CTexture> _Tex) { m_BloomTex = _Tex; }
    void SetRenderTargetTex(Ptr<CTexture> _Tex) { m_RenderTarget = _Tex; }
    void SetStrength(float _val) { m_fStrength = _val; }

public:
    virtual int UpdatePipeline() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    CCombine();
    ~CCombine();
};

