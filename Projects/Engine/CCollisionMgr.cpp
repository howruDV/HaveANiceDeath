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

	// 이름에 해당하는 Layer 가 존재하지 않으면
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

		// Left와 Right가 동일 레이어인 경우 이중 검사 방지
		// left 기준, vector idx의 우측으로만 검사
		size_t j = (_left == _right) ? i + 1 : 0;

		for (; j < vecRight.size(); ++j)
		{
			if (not vecRight[j]->Collider2D())
				continue;

			CollisionID ID = {};
			ID.LeftID = vecLeft[i]->Collider2D()->GetID();
			ID.RightID = vecRight[j]->Collider2D()->GetID();

			// 이전 프레임 충돌 정보
			map<UINT_PTR, bool>::iterator iter = m_ColPrevInfo.find(ID.id);
			if (iter == m_ColPrevInfo.end())
			{
				m_ColPrevInfo.insert(make_pair(ID.id, false));
				iter = m_ColPrevInfo.find(ID.id);
			}

			// 현재 프레임 충돌 처리
			bool IsDead = vecLeft[i]->IsDead() || vecRight[j]->IsDead();
			bool IsActive = vecLeft[i]->Collider2D()->IsActive() && vecRight[j]->Collider2D()->IsActive();
			bool IsSemiDeactive = vecLeft[i]->Collider2D()->IsSemiDeactive() || vecRight[j]->Collider2D()->IsSemiDeactive();

			// 두 충돌체중 하나라도 활성화 상태 아니라면, 충돌을 검사하지 않는다.
			if (!IsActive)
				return;

			// case: 현재 프레임 충돌
			if (CollisionBtwCollider2D(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
			{
				// case: 이전에도 충돌 (계속 충돌)
				if (iter->second)
				{
					// 둘중 하나가 죽을 예정이거나, 비활성화 예정 상태라면
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

				// case: 이전에 충돌아님 (새로 충돌)
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

			// case: 현재 프레임 충돌 아님
			else
			{
				// case: 이전에 충돌 (이번에 떨어짐)
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
	// 회전 고려, 투영시켜 충돌 검사(OBB)
	// -----------------------------
	// Rect Local
	// 0 -- 1
	// |    |
	// 3 -- 2
	static Vec3 arrRect[4] = { Vec3(-0.5f, 0.5f, 0.f)
							  , Vec3(0.5f, 0.5f, 0.f)
							  , Vec3(0.5f, -0.5f, 0.f)
							  , Vec3(-0.5f, -0.5f, 0.f) };
	
	// 두 rect collider의 표면벡터
	Vec3 arrProj[4] = {};
	arrProj[0] = XMVector3TransformCoord(arrRect[1], matLeft) - XMVector3TransformCoord(arrRect[0], matLeft); // XMVector3TransformCoord: vector에 변환행렬 곱해 w에는 1 넣어 반환
	arrProj[1] = XMVector3TransformCoord(arrRect[3], matLeft) - XMVector3TransformCoord(arrRect[0], matLeft);
	arrProj[2] = XMVector3TransformCoord(arrRect[1], matRight) - XMVector3TransformCoord(arrRect[0], matRight);
	arrProj[3] = XMVector3TransformCoord(arrRect[3], matRight) - XMVector3TransformCoord(arrRect[0], matRight);

	// 두 rec collider 중심 사이 벡터
	Vec3 vCenter = XMVector3TransformCoord(Vec3(), matRight) - XMVector3TransformCoord(Vec3(), matLeft);
	
	// 투영
	for (int i = 0; i < 4; ++i)
	{
		// 투영축
		Vec3 vProj = arrProj[i];
		vProj.Normalize();

		// 4개의 표면벡터 투영
		float ProjAcc = 0.f;
		
		for (int j = 0; j < 4; ++j)
			ProjAcc += abs(vProj.Dot(arrProj[j]));
		ProjAcc *= 0.5f;

		// 두 충돌체의 중심 사이 거리 투영
		float fCenterDist = abs(vProj.Dot(vCenter));

		// 충돌 검사
		if (ProjAcc < fCenterDist)
			return false;
	}
	// 어떤 축으로도 분리 불가능하면 충돌임
	return true;
}
