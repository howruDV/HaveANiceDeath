#pragma once
#include "CRenderComponent.h"
#include "CParticleUpdate.h"
#include "CTexture.h"
// =======================================
// CParticleSystem: Particle Group�� ����, �����ϴ� Component
// =======================================
// - ParticleSystem�� ������ Particles�� �̷���� �����Ƿ� CPU�� �ƴ� GPU���� �����ؾ� ��
// - ���� CompouteShader ���

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    
    CStructuredBuffer*      m_ParticleBuffer;
    UINT                    m_ParticleCountMax;
    Ptr<CTexture>           m_ParticleTex;
    
    // Compute Shader
    Ptr<CParticleUpdate>    m_CSParticleUpdate;

    // Module
    FParticleModule         m_Module;
    CStructuredBuffer*      m_ParticleModuleBuffer;

    // Module: Spawn
    CStructuredBuffer*      m_SpawnCountBuffer;
    float                   m_SpawnAccTime;

public:
    virtual void UpdatePipeline() override;
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CParticleSystem);
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _OriginParticle);
    ~CParticleSystem();
};

