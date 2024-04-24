#pragma once
#include "CRenderComponent.h"
#include "CParticleUpdate.h"
#include "CTexture.h"
// =======================================
// CParticleSystem: Particle Group을 생성, 관리하는 Component
// =======================================
// - ParticleSystem은 수많은 Particles로 이루어져 있으므로 CPU가 아닌 GPU에서 연산해야 함
// - 따라서 CompouteShader 사용

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*      m_ParticleBuffer;
    UINT                    m_ParticleSpawnMax;
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
    void SetParticleSpawnMax(UINT _Max) { m_ParticleSpawnMax = _Max; }
    void SetParticleTex(Ptr<CTexture> _Tex) { m_ParticleTex = _Tex; }
    void SetModule(FParticleModule _Module) { m_Module = _Module; }

    UINT GetParticleSpawnMax() { return m_ParticleSpawnMax; }
    Ptr<CTexture> GetParticleTex() { return m_ParticleTex; }
    FParticleModule GetModule() { return m_Module; }

public:
    CLONE(CParticleSystem);
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _OriginParticle);
    ~CParticleSystem();
};

