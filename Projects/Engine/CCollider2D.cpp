#include "pch.h"
#include "CCollider2D.h"
#include "CTransform.h"
#include "CScript.h"
#include "CTaskMgr.h"

CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_Type(COLLIDER2D_TYPE::RECT)
	, m_vOffsetScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(false)
	, m_Active(true)
	, m_SemiDeactive(false)
	, m_CollisionCount(0)
{
}

CCollider2D::CCollider2D(const CCollider2D& _OriginCollider2D)
	: CComponent(_OriginCollider2D)
	, m_Type(_OriginCollider2D.m_Type)
	, m_vOffsetPos(_OriginCollider2D.m_vOffsetPos)
	, m_vOffsetScale(_OriginCollider2D.m_vOffsetScale)
	, m_vOffestRotation(_OriginCollider2D.m_vOffestRotation)
	, m_matColWorld(_OriginCollider2D.m_matColWorld)
	, m_bAbsolute(_OriginCollider2D.m_bAbsolute)
	, m_CollisionCount(0)
	, m_Active(_OriginCollider2D.m_Active)
	, m_SemiDeactive(false)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::finaltick()
{
	// 비활성화 예정 상태면, 비활성화 시킨다.
	if (m_SemiDeactive)
	{
		FTask task = {};
		task.Type = TASK_TYPE::COLLIDER2D_DEACTIVE;
		task.Param_1 = (DWORD_PTR)this;
		CTaskMgr::GetInst()->AddTask(task);
	}
	// 비활성화된 충돌체는 계산하지 않는다.
	else if (!m_Active)
		return;

	// 최종 위치값 계산
	Vec3 vObjPos = Transform()->GetRelativePos();
	m_FinalPos = vObjPos + m_vOffsetPos;

	// 1. matrix calculate
	// collider local matrix
	m_matColWorld = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matColWorld *= XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	// owner's world matrix
	const Matrix& matObjWorld = Transform()->GetWorldMat();

	// collder world matrix
	if (m_bAbsolute)
	{
		Vec3 vWorldScale = Transform()->GetWorldScale();
		Matrix matWorldScaleInv = XMMatrixScaling(vWorldScale.x, vWorldScale.y, vWorldScale.z);
		matWorldScaleInv = XMMatrixInverse(nullptr, matWorldScaleInv);

		m_matColWorld = m_matColWorld * matWorldScaleInv * matObjWorld;
	}
	else
		m_matColWorld *= matObjWorld;

	// 2. render color
	if (m_CollisionCount == 0)
		GamePlayStatic::DrawDebugRect(m_matColWorld, Vec3(0.f, 1.f, 0.f), false);
	else if (1 <= m_CollisionCount)
		GamePlayStatic::DrawDebugRect(m_matColWorld, Vec3(1.f, 0.f, 0.f), false);
	else
		assert(nullptr);
}

void CCollider2D::BeginOverlap(CCollider2D* _OtherCollider)
{
	++m_CollisionCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
		vecScript[i]->BeginOverlap(this, _OtherCollider->GetOwner(), _OtherCollider);
}

void CCollider2D::Overlap(CCollider2D* _OtherCollider)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
		vecScript[i]->Overlap(this, _OtherCollider->GetOwner(), _OtherCollider);
}

void CCollider2D::EndOverlap(CCollider2D* _OtherCollider)
{
	if (GetOwner()->GetName() == L"KoTFlying")
	  		int a = 0;

	--m_CollisionCount;
	
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
		vecScript[i]->EndOverlap(this, _OtherCollider->GetOwner(), _OtherCollider);
}

void CCollider2D::SaveToFile(FILE* _File)
{
	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
	fwrite(&m_Type, sizeof(UINT), 1, _File);
}

void CCollider2D::LoadFromFile(FILE* _File)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
	fread(&m_Type, sizeof(UINT), 1, _File);
}

void CCollider2D::Deactivate()
{
	FTask task = {};
	task.Type = TASK_TYPE::COLLIDER2D_SEMI_DEACTIVE;
	task.Param_1 = (DWORD_PTR)this;
	CTaskMgr::GetInst()->AddTask(task);
}

Vec3 CCollider2D::GetFinalScale()
{
	if (m_bAbsolute)
		return m_vOffsetScale;

	if (GetOwner()->GetParent())
		return GetOwner()->GetParent()->Transform()->GetWorldScale() * m_vOffsetScale;

	return Transform()->GetWorldScale() * m_vOffsetScale;
}