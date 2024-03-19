#include "pch.h"
#include "CCollisionMgr.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"

CCollisionMgr::CCollisionMgr()
	: m_Matrix{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::tick()
{
	for (UINT iRow = 0; iRow < LAYER_MAX; ++iRow)
	{
		for (UINT iCol = iRow; iCol < LAYER_MAX; ++iCol)
		{
			if (m_Matrix[iRow] & 1 << iCol)
				CollisionBtwLayer(iRow, iCol);
		}
	}
}

void CCollisionMgr::LayerCheck(UINT _left, UINT _right)
{
	UINT iRow = _left;
	UINT iCol = _right;
	
	if (iRow>iCol)
	{
		UINT iTemp = iCol;
		iCol = iRow;
		iRow = iCol;
	}

	UINT CheckBit = (1 << iCol);

	if (m_Matrix[iRow] & CheckBit)
	{
		m_Matrix[iRow] &= ~CheckBit;
	}
	else
	{
		m_Matrix[iRow] |= CheckBit;
	}
}

void CCollisionMgr::LayerCheck(const wstring& _LeftLayer, const wstring& _RightLayer)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLeftLayer = pCurLevel->GetLayer(_LeftLayer);
	CLayer* pRightLayer = pCurLevel->GetLayer(_RightLayer);

	// �̸��� �ش��ϴ� Layer �� �������� ������
	assert(pLeftLayer && pRightLayer);

	LayerCheck(pLeftLayer->GetLayerIdx(), pRightLayer->GetLayerIdx());
}

void CCollisionMgr::Clear()
{
	for (int i = 0; i < LAYER_MAX; ++i)
		m_Matrix[i] = 0;
}

void CCollisionMgr::CollisionBtwLayer(UINT _left, UINT _right)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLeftLayer = pCurLevel->GetLayer(_left);
	CLayer* pRightLayer = pCurLevel->GetLayer(_right);
	const vector<CGameObject*>& vecLeft = pLeftLayer->GetLayerObjects();
	const vector<CGameObject*>& vecRight = pRightLayer->GetLayerObjects();

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (not vecLeft[i]->Collider2D())
			continue;

		// Left�� Right�� ���� ���̾��� ��� ���� �˻� ����
		// left ����, vector idx�� �������θ� �˻�
		size_t j = (_left == _right) ? i + 1 : 0;

		for (; j < vecRight.size(); ++j)
		{
			if (not vecRight[j]->Collider2D())
				continue;

			CollisionID ID = {};
			ID.LeftID = vecLeft[i]->Collider2D()->GetID();
			ID.RightID = vecRight[j]->Collider2D()->GetID();

			// ���� ������ �浹 ����
			map<UINT_PTR, bool>::iterator iter = m_ColPrevInfo.find(ID.id);
			if (iter == m_ColPrevInfo.end())
			{
				m_ColPrevInfo.insert(make_pair(ID.id, false));
				iter = m_ColPrevInfo.find(ID.id);
			}

			// ���� ������ �浹 ó��
			bool IsDead = vecLeft[i]->IsDead() || vecRight[j]->IsDead();
			bool IsActive = vecLeft[i]->Collider2D()->IsActive() && vecRight[j]->Collider2D()->IsActive();
			bool IsSemiDeactive = vecLeft[i]->Collider2D()->IsSemiDeactive() || vecRight[j]->Collider2D()->IsSemiDeactive();

			// �� �浹ü�� �ϳ��� Ȱ��ȭ ���� �ƴ϶��, �浹�� �˻����� �ʴ´�.
			if (!IsActive)
				return;

			// case: ���� ������ �浹
			if (CollisionBtwCollider2D(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
			{
				// case: �������� �浹 (��� �浹)
				if (iter->second)
				{
					// ���� �ϳ��� ���� �����̰ų�, ��Ȱ��ȭ ���� ���¶��
					if (IsDead || IsSemiDeactive)
					{
						vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
						vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
						iter->second = false;
					}
					else
					{
						vecLeft[i]->Collider2D()->Overlap(vecRight[j]->Collider2D());
						vecRight[j]->Collider2D()->Overlap(vecLeft[i]->Collider2D());
					}
				}

				// case: ������ �浹�ƴ� (���� �浹)
				else
				{
					if (!IsDead && !IsSemiDeactive)
					{
						vecLeft[i]->Collider2D()->BeginOverlap(vecRight[j]->Collider2D());
						vecRight[j]->Collider2D()->BeginOverlap(vecLeft[i]->Collider2D());
						iter->second = true;
					}
				}
			}

			// case: ���� ������ �浹 �ƴ�
			else
			{
				// case: ������ �浹 (�̹��� ������)
				if (iter->second)
				{
					vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
				}
				
				iter->second = false;
			}
		}
	}
}

bool CCollisionMgr::CollisionBtwCollider2D(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	const Matrix& matLeft = _pLeft->GetWorldMat();
	const Matrix& matRight = _pRight->GetWorldMat();

	// -----------------------------
	// ȸ�� ���, �������� �浹 �˻�(OBB)
	// -----------------------------
	// Rect Local
	// 0 -- 1
	// |    |
	// 3 -- 2
	static Vec3 arrRect[4] = { Vec3(-0.5f, 0.5f, 0.f)
							  , Vec3(0.5f, 0.5f, 0.f)
							  , Vec3(0.5f, -0.5f, 0.f)
							  , Vec3(-0.5f, -0.5f, 0.f) };
	
	// �� rect collider�� ǥ�麤��
	Vec3 arrProj[4] = {};
	arrProj[0] = XMVector3TransformCoord(arrRect[1], matLeft) - XMVector3TransformCoord(arrRect[0], matLeft); // XMVector3TransformCoord: vector�� ��ȯ��� ���� w���� 1 �־� ��ȯ
	arrProj[1] = XMVector3TransformCoord(arrRect[3], matLeft) - XMVector3TransformCoord(arrRect[0], matLeft);
	arrProj[2] = XMVector3TransformCoord(arrRect[1], matRight) - XMVector3TransformCoord(arrRect[0], matRight);
	arrProj[3] = XMVector3TransformCoord(arrRect[3], matRight) - XMVector3TransformCoord(arrRect[0], matRight);

	// �� rec collider �߽� ���� ����
	Vec3 vCenter = XMVector3TransformCoord(Vec3(), matRight) - XMVector3TransformCoord(Vec3(), matLeft);
	
	// ����
	for (int i = 0; i < 4; ++i)
	{
		// ������
		Vec3 vProj = arrProj[i];
		vProj.Normalize();

		// 4���� ǥ�麤�� ����
		float ProjAcc = 0.f;
		
		for (int j = 0; j < 4; ++j)
			ProjAcc += abs(vProj.Dot(arrProj[j]));
		ProjAcc *= 0.5f;

		// �� �浹ü�� �߽� ���� �Ÿ� ����
		float fCenterDist = abs(vProj.Dot(vCenter));

		// �浹 �˻�
		if (ProjAcc < fCenterDist)
			return false;
	}
	// � �����ε� �и� �Ұ����ϸ� �浹��
	return true;
}
