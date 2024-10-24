#include "pch.h"
#include "CParticleUpdate.h"

CParticleUpdate::CParticleUpdate()
	: CComputeShader(32, 1, 1)
	, m_ParticleBuffer(nullptr)
	, m_ParticleModuleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_fParticleRotZ(0.f)
	, m_fAnimDuration(0.f)
{
	Create(L"shader\\particle_update.fx", "CS_UpdateParticle");
}

CParticleUpdate::~CParticleUpdate()
{
}

int CParticleUpdate::UpdatePipeline()
{
	if (!m_ParticleBuffer)
		return E_FAIL;

	m_Const.iArr[0] = m_ParticleBuffer->GetElementCount();	// max count
	m_Const.iArr[1] = m_fParticleRotZ;	// Particle Texture Rotation
	m_Const.fArr[0] = m_fAnimDuration;
	m_Const.v4Arr[0] = m_vParticleWorldPos;

	m_ParticleBuffer->UpdateCS_UAV(0);
	m_SpawnCountBuffer->UpdateCS_UAV(1);
	m_ParticleModuleBuffer->UpdateCS_SRV(20);
	//m_AnimDurationBuffer->UpdateCS_SRV(21);

	return S_OK;
}

void CParticleUpdate::UpdateGroupCount()
{
	UINT GroupX = (m_ParticleBuffer->GetElementCount() / m_ThreadX);
	if (m_ParticleBuffer->GetElementCount() % m_ThreadX != 0)
		GroupX += 1;

	SetGroup(GroupX, 1, 1);
}

void CParticleUpdate::Clear()
{
	m_ParticleBuffer->ClearCS_UAV();
	m_ParticleBuffer = nullptr;

	m_ParticleModuleBuffer->ClearCS_SRV();
	m_ParticleModuleBuffer = nullptr;

	//m_AnimDurationBuffer->ClearCS_SRV();
	//m_AnimDurationBuffer = nullptr;

	m_SpawnCountBuffer->ClearCS_UAV();
	m_SpawnCountBuffer = nullptr;
}