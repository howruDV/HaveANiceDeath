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
	, m_fParticleRot(0.f)
{
	// 전용 mesh & material 사용
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMat"));

	// particle buffer
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(FParticle), m_ParticleSpawnMax, SB_TYPE::READ_WRITE, true);	// 확인용으로 READ_WRITE (CPU 읽기 허용)
	
	// particle module
	m_ParticleModuleBuffer = new CStructuredBuffer;
	UINT ModuleResize = sizeof(FParticleModule);

	if (ModuleResize % 16 != 0)
		ModuleResize = 16 * (ModuleResize / 16 + 1);

	m_ParticleModuleBuffer->Create(ModuleResize, 1, SB_TYPE::READ_ONLY, true);

	// spawn count buffer
	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(FSpawnCount), 1, SB_TYPE::READ_WRITE, true);

	// animation duration buffer;
	//m_AnimDurationBuffer = new CStructuredBuffer;
	//m_AnimDurationBuffer->Create(sizeof(Vec4), 1, SB_TYPE::READ_ONLY, true);
	m_AnimOffsetBuffer = new CStructuredBuffer;
	m_AnimOffsetBuffer->Create(sizeof(float) * 4, 1, SB_TYPE::READ_ONLY, true);

	// compute shader for particle update
	m_CSParticleUpdate = (CParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleUpdateShader").Get();

	// -------------------------
	// Module Setting
	// -------------------------
	m_Module.GravityScale = Vec3(0.f, -980.f, 0.f);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _OriginParticle)
	: CRenderComponent(_OriginParticle)
	, m_ParticleBuffer(nullptr)
	, m_ParticleSpawnMax(_OriginParticle.m_ParticleSpawnMax)
	, m_Module(_OriginParticle.m_Module)
	, m_ParticleModuleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_AnimOffsetBuffer(nullptr)
	, m_CSParticleUpdate(_OriginParticle.m_CSParticleUpdate)
	, m_ParticleTex(_OriginParticle.m_ParticleTex)
	, m_SpawnAccTime(0.f)
	, m_fPlayTime(_OriginParticle.m_fPlayTime)
	, m_fPlayAccTime(0.f)
	, m_bSpawnRepeat(_OriginParticle.m_bSpawnRepeat)
	, m_bNextDeactive(false)
	, m_fParticleRot(_OriginParticle.m_fParticleRot)
{
	if (nullptr != _OriginParticle.m_ParticleBuffer)
		m_ParticleBuffer = _OriginParticle.m_ParticleBuffer->Clone();

	if (nullptr != _OriginParticle.m_ParticleModuleBuffer)
		m_ParticleModuleBuffer = _OriginParticle.m_ParticleModuleBuffer->Clone();

	//if (nullptr != _OriginParticle.m_AnimDurationBuffer)
	//	m_AnimDurationBuffer = _OriginParticle.m_AnimDurationBuffer->Clone();
	
	if (nullptr != _OriginParticle.m_AnimOffsetBuffer)
		m_AnimOffsetBuffer = _OriginParticle.m_AnimOffsetBuffer->Clone();

	if (nullptr != _OriginParticle.m_SpawnCountBuffer)
		m_SpawnCountBuffer = _OriginParticle.m_SpawnCountBuffer->Clone();
}

CParticleSystem::~CParticleSystem()
{
	if (m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (m_ParticleModuleBuffer)
		delete m_ParticleModuleBuffer;

	//if (m_AnimDurationBuffer)
	//	delete m_AnimDurationBuffer;
	 
	if (m_AnimOffsetBuffer)
		delete m_AnimOffsetBuffer;

	if (m_SpawnCountBuffer)
		delete m_SpawnCountBuffer;
}

void CParticleSystem::UpdatePipeline()
{
}

void CParticleSystem::begin()
{
	// Module: Animation
	if (Animator2D())
	{
		m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ANIMATION] = true;
		m_Module.AnimFrmSize = Animator2D()->GetCurAnim()->GetFrmSize();
		m_Module.AnimRepeat = Animator2D()->IsRepeat();

		// animation offset buffer
		vector<Vec4> vecOffset = Animator2D()->GetCurAnim()->GetOffsetList();
		//m_AnimOffsetBuffer->SetData(&vecOffset, m_Module.AnimFrmSize);
		m_AnimOffsetBuffer->SetData(vecOffset.data(), (UINT)vecOffset.size());
		m_CSParticleUpdate->SetAnimDuration(Animator2D()->GetCurAnimFrm().fDuration);

		// animation duration list buffer
		//vector<Vec4> Tmp;
		//for (float it : Animator2D()->GetCurAnim()->GetDurationList())
		//	Tmp.push_back(Vec4(it, 0.f, 0.f, 0.f));

		//m_AnimDurationBuffer->SetData(&Tmp, m_Module.AnimFrmSize);
	}
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

	// ----------------------
	// Cacluate
	// ----------------------
	m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
	m_CSParticleUpdate->SetParticleModuleBuffer(m_ParticleModuleBuffer);
	m_CSParticleUpdate->SetParticleSpawnCount(m_SpawnCountBuffer);
	//m_CSParticleUpdate->SetAnimDuration(m_AnimDurationBuffer);
	m_CSParticleUpdate->SetParticleWorldPos(Transform()->GetWorldPos());
	m_CSParticleUpdate->SetParticleRot(m_fParticleRot);

	m_CSParticleUpdate->Execute();
	//CheckSpawnCount();
}

void CParticleSystem::render()
{
	// view, projection 행렬 전달
	Transform()->UpdatePipeline();

	// 1. binding
	m_ParticleBuffer->UpdatePipeline(20);
	m_ParticleModuleBuffer->UpdatePipeline(21);
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_ParticleTex);
	GetMaterial()->UpdatePipeline();

	if (Animator2D())
	{
		m_AnimOffsetBuffer->UpdatePipeline(22);
		Animator2D()->UpdatePipeline();
	}
	else
	{
		Animator2D()->Clear();
	}

	// 2. rendering
	GetMesh()->renderInstanced(m_ParticleSpawnMax);

	// pipeline binding clear
	m_ParticleBuffer->Clear(20);
	m_ParticleModuleBuffer->Clear(21);
	m_AnimOffsetBuffer->Clear(22);
}

void CParticleSystem::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);

	// 파티클 최대 갯수 및 모듈 정보 저장
	fwrite(&m_ParticleSpawnMax, sizeof(UINT), 1, _File);
	fwrite(&m_Module, sizeof(FParticleModule_Prev), 1, _File);
	//fwrite(&m_fParticleRot, sizeof(float), 1, _File); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// 사용하던 CS 가 누군지 저장
	//SaveAssetRef(m_CSParticleUpdate, _File);

	// 파티클 입자 텍스쳐 정보 저장
	SaveAssetRef(m_ParticleTex, _File);

	fwrite(&m_fPlayTime, sizeof(float), 1, _File);
	fwrite(&m_bSpawnRepeat, sizeof(bool), 1, _File);
}

void CParticleSystem::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	// 파티클 최대 갯수 및 모듈 정보 로드
	fread(&m_ParticleSpawnMax, sizeof(UINT), 1, _File);
	FParticleModule_Prev Tmp;
	fread(&Tmp, sizeof(FParticleModule_Prev), 1, _File);

	m_Module.vSpawnColor = Tmp.vSpawnColor;
	m_Module.vSpawnScaleMin = Tmp.vSpawnScaleMin;
	m_Module.vSpawnScaleMax = Tmp.vSpawnScaleMax;
	m_Module.LifeMin = Tmp.LifeMin;
	m_Module.LifeMax = Tmp.LifeMax;
	m_Module.MassMin = Tmp.MassMin;
	m_Module.MassMax = Tmp.MassMax;
	m_Module.SpawnRate = Tmp.SpawnRate;
	m_Module.SpaceType = Tmp.SpaceType;			// 좌표계 (0: Local
	m_Module.SpawnShape = Tmp.SpawnShape;			// (0: Sphere, 1: B
	m_Module.Radius = Tmp.Radius;				// Spawn Shape - Sp
	m_Module.vSpawnBoxScale = Tmp.vSpawnBoxScale;		// Spawn Shape - Bo
	m_Module.DragTime = Tmp.DragTime;
	m_Module.vScaleRatio = Tmp.vScaleRatio;
	m_Module.AddVelocityType = Tmp.AddVelocityType;	// 0: from center, 
	m_Module.SpeedMin = Tmp.SpeedMin;
	m_Module.SpeedMax = Tmp.SpeedMax;
	m_Module.FixedAngle = Tmp.FixedAngle;
	m_Module.FixedDirection = Tmp.FixedDirection;
	m_Module.NosieForceScale = Tmp.NosieForceScale;
	m_Module.NosieForceTerm = Tmp.NosieForceTerm;
	m_Module.Gravity = Tmp.Gravity;			// 0: Off, 1: On
	m_Module.GravityScale = Tmp.GravityScale;
	m_Module.VelocityAlignment = Tmp.VelocityAlignment;	// 0: Off, 1: On 
	m_Module.AlphaBasedLife = Tmp.AlphaBasedLife;		// 0: Off, 1: Norma
	m_Module.AlphaMaxAge = Tmp.AlphaMaxAge;
	for (int i=0; i<8; ++i)
		m_Module.arrModuleCheck[i] = Tmp.arrModuleCheck[i];

	//fread(&m_fParticleRot, sizeof(float), 1, _File); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// 사용하던 CS 가 누군지 로드
	/*Ptr<CComputeShader> cs;
	LoadAssetRef(cs, _File);
	m_CSParticleUpdate = (CParticleUpdate*)cs.Get();*/

	// 파티클 입자 텍스쳐 정보 로드
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
	m_ParticleBuffer->Create(sizeof(FParticle), m_ParticleSpawnMax, SB_TYPE::READ_WRITE, true);	// 확인용으로 READ_WRITE (CPU 읽기 허용)
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
