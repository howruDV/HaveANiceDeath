#include "pch.h"
#include "CParticleSystem.h"
#include "CAssetMgr.h"
#include "CDevice.h"
#include "CTransform.h"
#include "CParticleUpdate.h"
#include "CTimeMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleCountMax(20)
	, m_ParticleBuffer(nullptr)
	, m_SpawnAccTime(0.f)
{
	// 전용 mesh & material 사용
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMat"));

	m_ParticleTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\particle\\CartoonSmoke.png");

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
	// 임시 paritcle system 생성
	//FParticle arrParticle[100] = {};
	//for (UINT i = 0; i < m_ParticleCountMax; ++i)
	//{
	//	arrParticle[i].vWorldPos = Vec3(vResol.x / (m_ParticleCountMax + 1) * (i + 1) - (vResol.x / 2.f), 0.f, 99.f);
	//	arrParticle[i].vWorldScale = Vec3(50.f, 50.f, 1.f);
	//	arrParticle[i].Active = 0;
	//}

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(FParticle), m_ParticleCountMax, SB_TYPE::READ_WRITE, true);	// 확인용으로 READ_WRITE (CPU 읽기 허용)
	
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
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = true;
	m_Module.vSpawnColor = Vec4(1.f, 0.f, 0.f, 1.f);
	m_Module.vSpawnScaleMin = Vec4(20.f, 20.f, 1.f, 1.f);
	m_Module.vSpawnScaleMax = Vec4(50.f, 50.f, 1.f, 1.f);
	m_Module.LifeMin = 3.f;
	m_Module.LifeMax = 5.f;
	m_Module.MassMin = 1.f;
	m_Module.MassMax = 1.f;
	m_Module.SpawnRate = 10;
	m_Module.SpaceType = 1;

	m_Module.SpawnShape = 0;
	m_Module.Radius = 100.f;									// spawn shape 0 : circle
	m_Module.vSpawnBoxScale = Vec4(500.f, 500.f, 0.f, 0.f);		// spawn shape 1 : box

	// Drag
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = false;
	m_Module.DragTime = 0.5f;

	// Scale
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = false;
	m_Module.vScaleRatio = Vec3(0.1f, 0.1f, 0.1f);

	// Add Velocity
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_Module.AddVelocityType = 0;
	m_Module.SpeedMin = 100;
	m_Module.SpeedMax = 200;
	m_Module.FixedDirection;
	m_Module.FixedAngle;

	// Nosie Force
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_Module.NosieForceScale = 50.f;
	m_Module.NosieForceTerm = 0.3f;

	// Caculate Force
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CACULATE_FORCE] = true;
	m_Module.Gravity = false;
	m_Module.GravityScale = Vec3(0.f, -980.f, 0.f);

	// Render
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = false;
	m_Module.VelocityAlignment = false;	// 속도에 따른 방향 정렬
	m_Module.AlphaBasedLife = 2;		// 0: Off, 1: NormalizedAge, 2: Age						
	m_Module.AlphaMaxAge = 2.f;
}

CParticleSystem::CParticleSystem(const CParticleSystem& _OriginParticle)
	: CRenderComponent(_OriginParticle)
	, m_ParticleBuffer(nullptr)
	, m_ParticleCountMax(_OriginParticle.m_ParticleCountMax)
	, m_Module(_OriginParticle.m_Module)
	, m_ParticleModuleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_CSParticleUpdate(_OriginParticle.m_CSParticleUpdate)
	, m_ParticleTex(_OriginParticle.m_ParticleTex)
	, m_SpawnAccTime(0.f)
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
	// Set Module
	// ----------------------
	// Module: Spawn
	m_SpawnAccTime += DT;

	if (m_SpawnAccTime > 1.f / m_Module.SpawnRate)
	{
		// 누적 처리
		// - frame이 떨어지면 : DT를 누적시켰다가 한번에 처리해야 함
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

	FParticle arrParticle[20] = {};
	m_ParticleBuffer->GetData(arrParticle);
}

void CParticleSystem::render()
{
	// view, projection 행렬 전달
	Transform()->UpdatePipeline();

	// 1. binding
	m_ParticleBuffer->UpdatePipeline(20);
	m_ParticleModuleBuffer->UpdatePipeline(21);

	// 2. rendering
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_ParticleTex);
	GetMaterial()->UpdatePipeline();
	GetMesh()->renderInstanced(m_ParticleCountMax);

	// pipeline binding clear
	m_ParticleBuffer->Clear(20);
	m_ParticleModuleBuffer->Clear(21);
}

void CParticleSystem::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);

	// 파티클 최대 갯수 및 모듈 정보 저장
	fwrite(&m_ParticleCountMax, sizeof(UINT), 1, _File);
	fwrite(&m_Module, sizeof(FParticleModule), 1, _File);

	// 사용하던 CS 가 누군지 저장
	//SaveAssetRef(m_CSParticleUpdate, _File);

	// 파티클 입자 텍스쳐 정보 저장
	SaveAssetRef(m_ParticleTex, _File);
}

void CParticleSystem::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	// 파티클 최대 갯수 및 모듈 정보 로드
	fread(&m_ParticleCountMax, sizeof(UINT), 1, _File);
	fread(&m_Module, sizeof(FParticleModule), 1, _File);

	// 사용하던 CS 가 누군지 로드
	/*Ptr<CComputeShader> cs;
	LoadAssetRef(cs, _File);
	m_CSParticleUpdate = (CParticleUpdate*)cs.Get();*/

	// 파티클 입자 텍스쳐 정보 로드
	LoadAssetRef(m_ParticleTex, _File);
}