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
    UINT                    m_ParticleSpawnMax;
    Ptr<CTexture>           m_ParticleTex;
    float                   m_fParticleRot;
    
    // Compute Shader
    Ptr<CParticleUpdate>    m_CSParticleUpdate;

    // Module
    FParticleModule         m_Module;
    CStructuredBuffer*      m_ParticleModuleBuffer;

    // Module: Spawn
    CStructuredBuffer*      m_SpawnCountBuffer;
    float                   m_SpawnAccTime;
    float                   m_fPlayTime;
    float                   m_fPlayAccTime;
    bool                    m_bSpawnRepeat;
    bool                    m_bNextDeactive;

    // Module: Anim
    //CStructuredBuffer*      m_AnimDurationBuffer;
    CStructuredBuffer*      m_AnimOffsetBuffer;

public:
    virtual void UpdatePipeline() override;
    virtual void begin() override;
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void Play() { m_fPlayAccTime = 0.f; }
    UINT CheckSpawnCount();

public:
    void CreateParticleBuffer();
    void SpawnOn();
    void SpawnOff();
    void SetParticleSpawnMax(UINT _Max) { m_ParticleSpawnMax = _Max; CreateParticleBuffer(); }
    void SetParticleTex(Ptr<CTexture> _Tex) { m_ParticleTex = _Tex; }
    void SetModule(FParticleModule _Module) { m_Module = _Module; }
    void SetRotation(float _Rotation) { m_fParticleRot = _Rotation; }
    void SetPlayTime(float _Time) { m_fPlayTime = _Time; }
    void SetRepeat(bool _Repeat) { m_bSpawnRepeat = _Repeat; }

    UINT GetParticleSpawnMax() { return m_ParticleSpawnMax; }
    Ptr<CTexture> GetParticleTex() { return m_ParticleTex; }
    FParticleModule GetModule() { return m_Module; }
    float GetRotation() { return m_fParticleRot; }
    float GetPlayTime() { return m_fPlayTime; }
    bool IsRepeat() { return m_bSpawnRepeat; }

public:
    CLONE(CParticleSystem);
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _OriginParticle);
    ~CParticleSystem();
};

