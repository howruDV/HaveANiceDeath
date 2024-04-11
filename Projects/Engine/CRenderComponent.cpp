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
	// case: ������ dynamic material ������� ���
	if (_OriginRenderCom.m_SharedMat.Get() != _OriginRenderCom.m_CurMat.Get())
	{
		assert(_OriginRenderCom.m_DynamicMat.Get());

		// �����ϴ� render component�� dynamic material ����
		GetDynamicMaterial();
		m_DynamicMat = _OriginRenderCom.m_DynamicMat; // ���� ������ ���������� ���� �������� �� ���� // @TODO �̰Ÿ³�
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
	// ������ ����Ǹ� ������ ���纻 �޾Ƶ� DynamicMaterial �� �����Ѵ�.
	m_CurMat = m_SharedMat = _Mat;
	m_DynamicMat = nullptr;

	GetDynamicMaterial();
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// @TODO �����鿩��
	//CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	//if (!pCurLevel || pCurLevel->GetState() != LEVEL_STATE::PLAY)
		//return nullptr;

	// case: dynamic material�� ������ ���
	if (m_DynamicMat.Get())
		return m_DynamicMat;

	// case: shared material�� �ִ� ���
	if (m_SharedMat.Get())
	{
		// ���������� �����ؼ� ���������� ����� �װ� ���� ��������� �����Ѵ�.
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