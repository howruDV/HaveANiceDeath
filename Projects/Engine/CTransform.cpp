#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(true)
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	// --------------------------
	// Transformation Matrix
	// --------------------------
	m_matWorld = XMMatrixIdentity(); // �������
	
	// scale
	/*
	m_matWorld._11 = m_vRelativeScale.x;
	m_matWorld._22 = m_vRelativeScale.y;
	m_matWorld._33 = m_vRelativeScale.z;
	*/
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	// rotate
	Matrix matRotX = XMMatrixRotationX(m_vRelativeRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vRelativeRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRelativeRotation.z);

	// translation
	/*
	m_matWorld._41 = m_vRelativePos.x;
	m_matWorld._42 = m_vRelativePos.y;
	m_matWorld._43 = m_vRelativePos.z;
	*/
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

	// --------------------------
	// Object Direction Vector
	// --------------------------
	static const Vec3 arrAxis[3] =
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	// vec3 -> vec4 Ȯ��
	// - XMVector3TransformCoord:	w�� 1�� Ȯ�� (�̵� ���� o)
	// - XMVector3TransformNormal:	w�� 0���� Ȯ�� (�̵� ���� x)
	for (int i = 0; i < 3; ++i)
	{
		m_arrLocalDir[i] = XMVector3TransformNormal(arrAxis[i], m_matWorld); // translation ���� �ʱ� ���� 0���� Ȯ��
		m_arrLocalDir[i].Normalize(); // scale ���� �ʱ� ���� ����ȭ
		m_arrWorldDir[i] = m_arrLocalDir[i]; // scale ���� �ʱ� ���� ����ȭ
	}


	// --------------------------
	// Hierachy Struectre
	// --------------------------
	if (GetOwner()->GetParent())
	{
		const Matrix& matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		// transformation
		if (m_bAbsolute)
		{
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();
			Matrix matParetScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);
			m_matWorld = m_matWorld * matParetScaleInv * matParentWorld;
		}
		else
			m_matWorld *= matParentWorld;

		// direction
		for (int i = 0; i < 3; ++i)
		{
			m_arrWorldDir[i] = XMVector3TransformNormal(arrAxis[i], m_matWorld);
			m_arrWorldDir[i].Normalize();	// m_matWorld�� ũ������ ���� �� �����Ƿ� ����ȭ
		}
	}
}

void CTransform::UpdatePipeline()
{
	g_Transform.matWorld = m_matWorld;
	g_Transform.matWV = g_Transform.matWorld * g_Transform.matView;
	g_Transform.matWVP = g_Transform.matWV * g_Transform.matProj;

	// ��ġ������ Transform Constant Buffer�� ������, B0 Register�� binding
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_Transform);
	pCB->UpdatePipeline();
}

Vec3 CTransform::GetWorldScale()
{
	CGameObject* pParent = GetOwner()->GetParent();
	Vec3 vWorldScale = m_vRelativeScale;

	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();
		pParent = pParent->GetParent();
	}

	return vWorldScale;
}

void CTransform::SaveToFile(FILE* _File)
{
	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
}