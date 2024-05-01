#include "pch.h"
#include "CParticleSystem.h"
#include "CAssetMgr.h"
#include "CDevice.h"
#include "CTransform.h"
#include "CParticleUpdate.h"
#include "CTimeMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleSpawnMax(20)
	, m_ParticleBuffer(nullptr)
	, m_SpawnAccTime(0.f)
	, m_fPlayTime(0.f)
	, m_fPlayAccTime(0.f)
	, m_bSpawnRepeat(true)
	, m_bNextDeactive(false)
{
	// ���� mesh & material ���
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMat"));

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(FParticle), m_ParticleSpawnMax, SB_TYPE::READ_WRITE, true);	// Ȯ�ο����� READ_WRITE (CPU �б� ���)
	
	m_ParticleModuleBuffer = new CStructuredBuffer;
	UINT ModuleResize = sizeof(FParticleModule);
	if (ModuleResize % 16 != 0)
		ModuleResize = 16 * (ModuleResize / 16 + 1);
	m_ParticleModuleBuffer->Create(ModuleResize, 1, SB_TYPE::READ_ONLY, true);

	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(FSpawnCount), 1, SB_TYPE::READ_WRITE, true);

	// compute shader for particle update
	m_CSParticleUpdate = (CParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleUpdateShader").Get();

	// -------------------------
	// Module Setting
	// -------------------------
	// Spawn
	//m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = true;
	//m_Module.vSpawnColor = Vec4(1.f, 0.f, 0.f, 1.f);
	//m_Module.vSpawnScaleMin = Vec4(20.f, 20.f, 1.f, 1.f);
	//m_Module.vSpawnScaleMax = Vec4(50.f, 50.f, 1.f, 1.f);
	//m_Module.LifeMin = 3.f;
	//m_Module.LifeMax = 5.f;
	//m_Module.MassMin = 1.f;
	//m_Module.MassMax = 1.f;
	//m_Module.SpawnRate = 10;
	//m_Module.SpaceType = 1;

	//m_Module.SpawnShape = 0;
	//m_Module.Radius = 100.f;									// spawn shape 0 : circle
	//m_Module.vSpawnBoxScale = Vec4(500.f, 500.f, 0.f, 0.f);		// spawn shape 1 : box

	//// Drag
	//m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = false;
	//m_Module.DragTime = 0.5f;

	//// Scale
	//m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = false;
	//m_Module.vScaleRatio = Vec3(0.1f, 0.1f, 0.1f);

	//// Add Velocity
	//m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	//m_Module.AddVelocityType = 0;
	//m_Module.SpeedMin = 100;
	//m_Module.SpeedMax = 200;
	//m_Module.FixedDirection;
	//m_Module.FixedAngle;

	//// Nosie Force
	//m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	//m_Module.NosieForceScale = 50.f;
	//m_Module.NosieForceTerm = 0.3f;

	//// Caculate Force
	//m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CACULATE_FORCE] = true;
	//m_Module.Gravity = false;
	m_Module.GravityScale = Vec3(0.f, -980.f, 0.f);

	//// Render
	//m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = false;
	//m_Module.VelocityAlignment = false;	// �ӵ��� ���� ���� ����
	//m_Module.AlphaBasedLife = 2;		// 0: Off, 1: NormalizedAge, 2: Age						
	//m_Module.AlphaMaxAge = 2.f;
}

CParticleSystem::CParticleSystem(const CParticleSystem& _OriginParticle)
	: CRenderComponent(_OriginParticle)
	, m_ParticleBuffer(nullptr)
	, m_ParticleSpawnMax(_OriginParticle.m_ParticleSpawnMax)
	, m_Module(_OriginParticle.m_Module)
	, m_ParticleModuleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_CSParticleUpdate(_OriginParticle.m_CSParticleUpdate)
	, m_ParticleTex(_OriginParticle.m_ParticleTex)
	, m_SpawnAccTime(0.f)
	, m_fPlayTime(_OriginParticle.m_fPlayTime)
	, m_fPlayAccTime(0.f)
	, m_bSpawnRepeat(_OriginParticle.m_bSpawnRepeat)
	, m_bNextDeactive(false)
{
	if (nullptr != _OriginParticle.m_ParticleBuffer)
		m_ParticleBuffer = _OriginParticle.m_ParticleBuffer->Clone();

	if (nullptr != _OriginParticle.m_ParticleModuleBuffer)
		m_ParticleModuleBuffer = _OriginParticle.m_ParticleModuleBuffer->Clone();

	if (nullptr != _OriginParticle.m_SpawnCountBuffer)
		m_SpawnCountBuffer = _OriginParticle.m_SpawnCountBuffer->Clone();
}

CParticleSystem::~CParticleSystem()
{
	if (m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (m_ParticleModuleBuffer)
		delete m_ParticleModuleBuffer;

	if (m_SpawnCountBuffer)
		delete m_SpawnCountBuffer;
}

void CParticleSystem::UpdatePipeline()
{
}

void CParticleSystem::finaltick()
{
	// ----------------------
	// Active
	// ----------------------
	if (!m_bSpawnRepeat || m_bNextDeactive)
	{
		m_fPlayAccTime += DT;

		if (m_fPlayAccTime > m_fPlayTime)
		{
			m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = false;

			if (m_bNextDeactive && (m_fPlayAccTime > m_Module.LifeMax) )
			{
				m_fPlayAccTime = 0.f;
				m_bNextDeactive = false;
				GetOwner()->Deactivate();
			}

			if (!m_bSpawnRepeat && (m_fPlayAccTime > m_fPlayTime + m_Module.LifeMax))
			{
				GamePlayStatic::DestroyGameObject(this->GetOwner());
			}
		}
	}

	// ----------------------
	// Set Module
	// ----------------------
	// Module: Spawn
	m_SpawnAccTime += DT;

	if (m_SpawnAccTime > 1.f / m_Module.SpawnRate)
	{
		// ���� ó��
		// - frame�� �������� : DT�� �������״ٰ� �ѹ��� ó���ؾ� ��
		float fSpawnCount = m_SpawnAccTime / (1.f / m_Module.SpawnRate);
		m_SpawnAccTime -= (1.f / (float)m_Module.SpawnRate) * floor(fSpawnCount);
		
		FSpawnCount count = FSpawnCount{ (int)fSpawnCount,0,0,0};
		m_SpawnCountBuffer->SetData(&count);
	}
	else
	{
		FSpawnCount count = FSpawnCount{ 0, };
		m_SpawnCountBuffer->SetData(&count);
	}

	// Update Module
	m_ParticleModuleBuffer->SetData(&m_Module);
	m_ParticleModuleBuffer->UpdateCS_SRV(20);

	// ----------------------
	// Cacluate
	// ----------------------
	m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
	m_CSParticleUpdate->SetParticleModuleBuffer(m_ParticleModuleBuffer);
	m_CSParticleUpdate->SetParticleSpawnCount(m_SpawnCountBuffer);
	m_CSParticleUpdate->SetParticleWorldPos(Transform()->GetWorldPos());

	m_CSParticleUpdate->Execute();
	//CheckSpawnCount();
}

void CParticleSystem::render()
{
	// view, projection ��� ����
	Transform()->UpdatePipeline();

	// 1. binding
	m_ParticleBuffer->UpdatePipeline(20);
	m_ParticleModuleBuffer->UpdatePipeline(21);
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_ParticleTex);
	GetMaterial()->UpdatePipeline();

	if (Animator2D())
		Animator2D()->UpdatePipeline();
	else
	{
		Animator2D()->Clear();
	}

	// 2. rendering
	GetMesh()->renderInstanced(m_ParticleSpawnMax);

	// pipeline binding clear
	m_ParticleBuffer->Clear(20);
	m_ParticleModuleBuffer->Clear(21);
}

void CParticleSystem::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);

	// ��ƼŬ �ִ� ���� �� ��� ���� ����
	fwrite(&m_ParticleSpawnMax, sizeof(UINT), 1, _File);
	fwrite(&m_Module, sizeof(FParticleModule), 1, _File);

	// ����ϴ� CS �� ������ ����
	//SaveAssetRef(m_CSParticleUpdate, _File);

	// ��ƼŬ ���� �ؽ��� ���� ����
	SaveAssetRef(m_ParticleTex, _File);

	fwrite(&m_fPlayTime, sizeof(float), 1, _File);
	fwrite(&m_bSpawnRepeat, sizeof(bool), 1, _File);
}

void CParticleSystem::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	// ��ƼŬ �ִ� ���� �� ��� ���� �ε�
	fread(&m_ParticleSpawnMax, sizeof(UINT), 1, _File);
	fread(&m_Module, sizeof(FParticleModule), 1, _File);

	// ����ϴ� CS �� ������ �ε�
	/*Ptr<CComputeShader> cs;
	LoadAssetRef(cs, _File);
	m_CSParticleUpdate = (CParticleUpdate*)cs.Get();*/

	// ��ƼŬ ���� �ؽ��� ���� �ε�
	LoadAssetRef(m_ParticleTex, _File);

	fread(&m_fPlayTime, sizeof(float), 1, _File);
	fread(&m_bSpawnRepeat, sizeof(bool), 1, _File);
}

UINT CParticleSystem::CheckSpawnCount()
{
	vector<FParticle> arrParticle ( m_ParticleSpawnMax );
	m_ParticleBuffer->GetData_ToVec(arrParticle);
	int Spawn = 0;

	for (FParticle& it : arrParticle)
	{
		if (it.Active)
			Spawn++;
	}

	return Spawn;
}

void CParticleSystem::CreateParticleBuffer()
{
	if (m_ParticleBuffer->GetElementCount() == m_ParticleSpawnMax)
		return;

	if (m_ParticleBuffer)
		delete m_ParticleBuffer;

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(FParticle), m_ParticleSpawnMax, SB_TYPE::READ_WRITE, true);	// Ȯ�ο����� READ_WRITE (CPU �б� ���)
}

void CParticleSystem::SpawnOn()
{
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = true;
	GetOwner()->Activate();
	m_bNextDeactive = false;
}

void CParticleSystem::SpawnOff()
{
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = false;
	m_bNextDeactive = true;
}
