#pragma once
#include "CComputeShader.h"
// =======================================
// CUpdateParticle: Particle Update Function
// =======================================

class CParticleUpdate :
    public CComputeShader
{
private:
    CStructuredBuffer*  m_ParticleBuffer;
    CStructuredBuffer*  m_ParticleModuleBuffer;
    CStructuredBuffer*  m_SpawnCountBuffer;
    //CStructuredBuffer*  m_AnimDurationBuffer;
    Vec3                m_vParticleWorldPos;
    float               m_fParticleRotZ;
    float               m_fAnimDuration;

public:
    virtual int UpdatePipeline() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    void SetParticleBuffer(CStructuredBuffer* _ParticleBuffer) { m_ParticleBuffer = _ParticleBuffer; }
    void SetParticleModuleBuffer(CStructuredBuffer* _ParticleBuffer) { m_ParticleModuleBuffer = _ParticleBuffer; }
    void SetParticleSpawnCount(CStructuredBuffer* _ParticleBuffer) { m_SpawnCountBuffer = _ParticleBuffer; }
    //void SetAnimDuration(CStructuredBuffer* _AnimDurationBuffer) { m_AnimDurationBuffer = _AnimDurationBuffer; }
    void SetAnimDuration(float _Duration) { m_fAnimDuration = _Duration; }
    void SetParticleWorldPos(Vec3 _vWorldPos) { m_vParticleWorldPos = _vWorldPos; }
    void SetParticleRot(float _Rotation) { m_fParticleRotZ = _Rotation; }

public:
    CParticleUpdate();
    ~CParticleUpdate();
};

