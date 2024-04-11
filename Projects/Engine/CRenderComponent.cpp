#include "pch.h"
#include "CRenderComponent.h"
#include "CLevelMgr.h"
#include "CLevel.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
	, m_Mesh(nullptr)
	, m_CurMat(nullptr)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _OriginRenderCom)
	: CComponent(_OriginRenderCom)
	, m_Mesh(_OriginRenderCom.m_Mesh)
	, m_SharedMat(_OriginRenderCom.m_SharedMat)
{
	// case: 원본이 dynamic material 사용중인 경우
	if (_OriginRenderCom.m_SharedMat.Get() != _OriginRenderCom.m_CurMat.Get())
	{
		assert(_OriginRenderCom.m_DynamicMat.Get());

		// 복사하는 render component도 dynamic material 생성
		GetDynamicMaterial();
		m_DynamicMat = _OriginRenderCom.m_DynamicMat; // 현재 생성한 동적재질로 원본 동적재질 값 복사 // @TODO 이거맞냐
	}
	else
	{
		m_CurMat = m_SharedMat;
	}
}

CRenderComponent::~CRenderComponent()
{
}

// Usage: use shared material
void CRenderComponent::RestoreMaterial()
{
	m_CurMat = m_SharedMat;
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mat)
{
	// 재질이 변경되면 기존에 복사본 받아둔 DynamicMaterial 을 삭제한다.
	m_CurMat = m_SharedMat = _Mat;
	m_DynamicMat = nullptr;

	GetDynamicMaterial();
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// @TODO 터지면여기
	//CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	//if (!pCurLevel || pCurLevel->GetState() != LEVEL_STATE::PLAY)
		//return nullptr;

	// case: dynamic material을 보유한 경우
	if (m_DynamicMat.Get())
		return m_DynamicMat;

	// case: shared material이 있는 경우
	if (m_SharedMat.Get())
	{
		// 공유재질을 복사해서 동적재질을 만들고 그걸 현재 사용재질로 설정한다.
		m_CurMat = m_DynamicMat = m_SharedMat->Clone();
		return m_DynamicMat;
	}

	return nullptr;
}

void CRenderComponent::begin()
{
	GetDynamicMaterial();
}

void CRenderComponent::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_SharedMat, _File);
}

void CRenderComponent::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Mesh, _File);
	LoadAssetRef(m_SharedMat, _File);

	SetMaterial(m_SharedMat);
}