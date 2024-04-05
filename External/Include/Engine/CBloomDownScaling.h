#pragma once
#include "CComputeShader.h"

class CBloomDownScaling :
    public CComputeShader
{
    Ptr<CTexture>   m_SourceTex;
    Ptr<CTexture>   m_TargetTex;

public:
    virtual int UpdatePipeline() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    void SetSourceTexture(Ptr<CTexture> _tex) { m_SourceTex = _tex; }
    void SetTargetTexture(Ptr<CTexture> _tex) { m_TargetTex = _tex; }

public:
    CBloomDownScaling();
    ~CBloomDownScaling();
};